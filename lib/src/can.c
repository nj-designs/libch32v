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

#include "can.h"
#include "core.h"
#include "rcc.h"

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

static void set_brp(struct CANRegMap *can_ctrl, uint32_t bus_speed) {
  uint32_t tmp32;
  uint32_t btr;
  const uint32_t ts_val = 3 + 2 + 3; // Assuming ->btimr default values

  btr = rcc_get_clk_freq(RCC_CLOCK_ID_PB1) / (ts_val * bus_speed);
  btr = 11;

  tmp32 = can_ctrl->btimr & ~CAN_BTIMR_BRP_MASK;
  tmp32 |= btr;

  can_ctrl->btimr = tmp32;
}

void can_init(struct CANRegMap *can_ctrl, uint32_t bus_speed, bool silent, bool loopback) {

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

  // Enter normal mode
  can_ctrl->ctlr = 0;
}

void can_deinit(struct CANRegMap *can_ctrl) { enbable_ctrl(can_ctrl, 0); }

void can_filter_init(struct CANRegMap *reg_ptr) {

  (void)reg_ptr;
  can1_filter.fctlr.finit = 1;

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

bool can_tx_req(struct CANTxReq *req) {

  // Find free mb
  req->_mb_idx = CAN_TX_MB_INVALID_IDX;
  if (req->reg_ptr->tstatr.tme0) {
    req->_mb_idx = 0;
  } else if (req->reg_ptr->tstatr.tme1) {
    req->_mb_idx = 1;
  } else if (req->reg_ptr->tstatr.tme2) {
    req->_mb_idx = 2;
  } else {
    return false;
  }

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
