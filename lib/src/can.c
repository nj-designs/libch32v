/**
 * @file can.c
 * @author Neil Johnson (nj.designs@protonmail.com)
 * @brief
 * @version 0.1
 * @date 2025-04-16
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <stdint.h>

#include "printf.h"

#include "can.h"
#include "core.h"
#include "rcc.h"

static can_rx_cb registered_can_rx_cb;

#ifdef LIBCH32_HAS_CAN1
struct CANRegMap __attribute__((section(".can1"))) can1;
struct CANMailboxRegMap __attribute__((section(".can1_mb"))) can1_mb;
struct CANFilterRegMap __attribute__((section(".can1_filter"))) can1_filter;
#endif

#ifdef LIBCH32_HAS_CAN2
struct CANRegMap __attribute__((section(".can2"))) can2;
struct CANMailboxRegMap __attribute__((section(".can2_mb"))) can2_mb;
#endif

static void enbable_ctrl(struct CANRegMap *reg_ptr, uint32_t on) {
#ifdef LIBCH32_HAS_CAN1
  if (reg_ptr == &can1) {
    rcc_set_peripheral_clk(RCCCan1Id, on);
    rcc_reset_peripherial(RCCCan1Id);
    return;
  }
#endif
#ifdef LIBCH32_HAS_CAN2
  if (reg_ptr == &can2) {
    rcc_set_peripheral_clk(RCCCan2Id, on);
    rcc_reset_peripherial(RCCCan2Id);
    return;
  }
#endif
}

static void __attribute__((noinline)) set_brp(struct CANRegMap *can_ctrl, uint32_t bus_speed) {
  volatile uint32_t tmp32;
  volatile uint32_t btr;
  const uint32_t ts_val = 3 + 2 + 3; // Assuming ->btimr default values

  volatile uint32_t clk_freq = rcc_get_clk_freq(RCC_CLOCK_ID_PB1);

  btr = clk_freq / (ts_val * bus_speed);

  tmp32 = can_ctrl->btimr & ~CAN_BTIMR_BRP_MASK;
  tmp32 |= btr;

  can_ctrl->btimr = tmp32;
}

void can_init(struct CANRegMap *can_ctrl, uint32_t bus_speed, bool silent, bool loopback, can_rx_cb rx_cb) {

  registered_can_rx_cb = rx_cb;

  enbable_ctrl(can_ctrl, 1);
  // Can controller enters SLEEP_MODE after reset, need to transition to
  // INIT_MODE
  can_ctrl->ctlr = CAN_CTRL_INRQ;
  while ((can_ctrl->statr & CAN_STATR_INAK) == 0) {
  };
  set_brp(can_ctrl, bus_speed);

  if (silent) {
    can_ctrl->btimr |= CAN_BTIMR_SILM;
  }
  if (loopback) {
    can_ctrl->btimr |= CAN_BTIMR_LBKM;
  }

  can_ctrl->intenr = CAN_INTENR_FMPIE0 | CAN_INTENR_FMPIE1;

  core_enable_pfic_irq(PFIC_CAN1_RX0_INT_NUM);
  core_enable_pfic_irq(PFIC_CAN1_RX1_INT_NUM);

  // Enter normal mode
  can_ctrl->ctlr = CAN_CTRL_TXFP;
}

void can_deinit(struct CANRegMap *can_ctrl) { enbable_ctrl(can_ctrl, 0); }

void can_filter_init(struct CANRegMap *reg_ptr) {

  (void)reg_ptr;

  for (int i = 0; i < 28; i++) {
    can1_filter.fb[i].fr1 = 0;
    can1_filter.fb[i].fr2 = 0;
  }

  can1_filter.fscfgr |= 1;
  can1_filter.fmcfgr = 0;

  can1_filter.fb[0].fr1 = 0;
  can1_filter.fb[0].fr2 = 0;

  can1_filter.fafifor = 0;
  can1_filter.fwr |= 1;

  can1_filter.fctlr.finit = 0;
}

union CANIdFilter {
  uint8_t bytes[4];
  uint32_t dword;
};

void can_filter_init_ex(struct CANRegMap *reg_ptr, const uint32_t *ids, uint32_t id_cnt) {

  uint32_t mb_start_idx;
  if (reg_ptr == CAN2) {
    mb_start_idx = can1_filter.fctlr.can2sb;
  } else {
    mb_start_idx = 0;
  }
  id_cnt = (id_cnt <= 14) ? id_cnt : 14;

  for (uint32_t i = 0; i < 28; i++) {
    can1_filter.fb[i].fr1 = 0;
    can1_filter.fb[i].fr2 = 0;
  }

  // Filter needs to be in init mode
  can1_filter.fctlr.finit = 1;

  for (uint32_t id_idx = 0; id_idx < id_cnt; id_idx++) {
    volatile uint32_t id = ids[id_idx];
    volatile union CANIdFilter fr = {.dword = 0};
    volatile uint32_t mb_idx = mb_start_idx + (id_idx / 2);

    fr.bytes[2] = (id & 0b111) << 5;
    fr.bytes[3] = (id >> 3);
    if (id & CAN_EXT_BIT) {
      fr.bytes[0] = ((id & 0b11111) << 3) | 0x02;
      fr.bytes[1] = (id >> 5);
      fr.bytes[2] |= ((id >> 13) & 0b11111);
    }

    if (id_idx & 1) {
      can1_filter.fafifor |= (1 << mb_idx); // FOFO 1
      can1_filter.fb[mb_idx].fr2 = fr.dword;
    } else {
      can1_filter.fb[mb_idx].fr1 = fr.dword;
    }

    can1_filter.fmcfgr |= (1 << mb_idx); // Id list mode
    can1_filter.fscfgr |= (1 << mb_idx); // Is single 32 bit
    can1_filter.fwr |= (1 << mb_idx);    // Is active
  }
  // Back to normal mode
  can1_filter.fctlr.finit = 0;
}

bool can_tx_req(struct CANTxReq *req, uint32_t max_wait_ms) {

  // Find free mb
  req->_mb_idx = CAN_TX_MB_INVALID_IDX;
retry:
  do {
    if (req->reg_ptr->tstatr.tme0) {
      req->_mb_idx = 0;
    } else if (req->reg_ptr->tstatr.tme1) {
      req->_mb_idx = 1;
    } else if (req->reg_ptr->tstatr.tme2) {
      req->_mb_idx = 2;
    } else if (max_wait_ms--) {
      core_delay_ms(1);
      goto retry;
    } else {
      return false;
    }
  } while (req->_mb_idx == CAN_TX_MB_INVALID_IDX);

  // Get mb for this controller
  struct CANMailboxRegMap *mb = (req->reg_ptr == CAN1) ? &can1_mb : &can2_mb;

  union CANMailboxTxMirRegBits mir = {.dword = 0};

  if (req->id & CAN_EXT_BIT) {
    mir.ide = 1;
    mir.exid = req->id & 0x3ffff;
    mir.stid = (req->id >> 18);
  } else {
    mir.stid = req->id;
  }
  mb->tx[req->_mb_idx].mir.dword = mir.dword;

  mb->tx[req->_mb_idx].mdtr &= (uint32_t)(~0b1111);
  mb->tx[req->_mb_idx].mdtr |= (req->data_len & 0b1111);
  mb->tx[req->_mb_idx].mdlr = (uint32_t)req->data_ptr[3] << 24 | (uint32_t)req->data_ptr[2] << 16 |
                              (uint32_t)req->data_ptr[1] << 8 | (uint32_t)req->data_ptr[0];
  mb->tx[req->_mb_idx].mdhr = (uint32_t)req->data_ptr[7] << 24 | (uint32_t)req->data_ptr[6] << 16 |
                              (uint32_t)req->data_ptr[5] << 8 | (uint32_t)req->data_ptr[4];

  mb->tx[req->_mb_idx].mir.txrq = 1;

  return true;
}

enum CanTxStatus can_check_tx_complete(const struct CANTxReq *req) {

  const uint32_t tstatr = req->reg_ptr->tstatr.dword >> req->_mb_idx * 8;

  if ((tstatr & CAN_TSTATR_RQCP) == 0) {
    return CAN_TX_RUNNING;
  }

  enum CanTxStatus status = (tstatr & CAN_TSTATR_TXOK) ? CAN_TX_DONE : CAN_TX_ERROR;

  req->reg_ptr->tstatr.dword |= (CAN_TSTATR_RQCP | CAN_TSTATR_TXOK | CAN_TSTATR_ALST | CAN_TSTATR_TERRO)
                                << req->_mb_idx * 8;

  return status;
}

uint32_t rx0_count;
uint32_t rx1_count;

#define RX_BUFFER_CNT 5

struct {
  uint32_t mir;
  uint32_t mdtr;
  uint32_t mdlr;
  uint32_t mdhr;
} rx0[RX_BUFFER_CNT];

struct {
  uint32_t mir;
  uint32_t mdtr;
  uint32_t mdlr;
  uint32_t mdhr;
} rx1[RX_BUFFER_CNT];

#ifdef LIBCH32_HAS_CAN1
void USB_LP_CAN1_RX0_IRQHandler(void) NJD_IRQ_ATTRIBUTE;
void USB_LP_CAN1_RX0_IRQHandler(void) {
  if (rx0_count < RX_BUFFER_CNT) {
    rx0[rx0_count].mir = can1_mb.rx[0].mir;
    rx0[rx0_count].mdtr = can1_mb.rx[0].mdtr;
    rx0[rx0_count].mdlr = can1_mb.rx[0].mdlr;
    rx0[rx0_count].mdhr = can1_mb.rx[0].mdhr;
    rx0_count++;
  }

  printf_("RX0\n");

  can1.rfifo0 = CAN_RFIFO0_RFOM0 | CAN_RFIFO0_FOVR0 | CAN_RFIFO0_FULL0;
}

void CAN1_RX1_IRQHandler(void) NJD_IRQ_ATTRIBUTE;
void CAN1_RX1_IRQHandler(void) {
  if (rx1_count < RX_BUFFER_CNT) {
    rx1[rx1_count].mir = can1_mb.rx[1].mir;
    rx1[rx1_count].mdtr = can1_mb.rx[1].mdtr;
    rx1[rx1_count].mdlr = can1_mb.rx[1].mdlr;
    rx1[rx1_count].mdhr = can1_mb.rx[1].mdhr;
    rx1_count++;
  }

  printf_("RX1\n");
  can1.rfifo1 = CAN_RFIFO1_RFOM1 | CAN_RFIFO1_FOVR1 | CAN_RFIFO1_FULL1;
}
#endif

#ifdef LIBCH32_HAS_CAN2
void CAN2_RX0_IRQHandler(void) NJD_IRQ_ATTRIBUTE;
void CAN2_RX0_IRQHandler(void) {}

void CAN2_RX1_IRQHandler(void) NJD_IRQ_ATTRIBUTE;
void CAN2_RX1_IRQHandler(void) {}
#endif
