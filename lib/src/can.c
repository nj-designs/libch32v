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

#if defined(LIBCH32_HAS_CAN1)

#include <stdint.h>

#include "printf.h"

#include "can.h"
#include "core.h"
#include "rcc.h"

struct CANRegMap {
  volatile uint32_t ctlr;  // 0x00
  volatile uint32_t statr; // 0x04
  volatile union {
    struct {
      uint32_t rqcp0    :1; // 0
      uint32_t txok0    :1; // 1
      uint32_t alst0    :1; // 2
      uint32_t terr0    :1; // 3
      uint32_t res6_4   :3; // 6:4
      uint32_t abrq0    :1; // 7
      uint32_t rqcp1    :1; // 8
      uint32_t txok1    :1; // 9
      uint32_t alst1    :1; // 10
      uint32_t terr1    :1; // 11
      uint32_t res14_12 :3; // 14:12
      uint32_t abrq1    :1; // 15
      uint32_t rqcp2    :1; // 16
      uint32_t tkok2    :1; // 17
      uint32_t alst2    :1; // 18
      uint32_t terr2    :1; // 19
      uint32_t res22_20 :3; // 22:20
      uint32_t abrq2    :1; // 23
      uint32_t code     :2; // 25:24
      uint32_t tme0     :1; // 26
      uint32_t tme1     :1; // 27
      uint32_t tme2     :1; // 28
      uint32_t low0     :1; // 29
      uint32_t low1     :1; // 30
      uint32_t low2     :1; // 31
    };
    uint32_t dword;
  } tstatr;
  volatile uint32_t rfifo0; // 0x0C
  volatile uint32_t rfifo1; // 0x10
  volatile uint32_t intenr; // 0x14
  volatile uint32_t errsr;  // 0x18
  volatile uint32_t btimr;  // 0x1C
  volatile uint32_t ttctlr; // 0x20
  volatile uint32_t ttcnt;  // 0x24
};

union CANMailboxTxMirRegBits {
  struct {
    uint32_t txrq :1;  // 0
    uint32_t rtr  :1;  // 1
    uint32_t ide  :1;  // 2
    uint32_t exid :18; // 20:3
    uint32_t stid :11; // 31:21
  };
  uint32_t dword;
};

struct CANMailboxRegMap {
  volatile struct {
    union CANMailboxTxMirRegBits mir;
    uint32_t                     mdtr;
    uint32_t                     mdlr;
    uint32_t                     mdhr;
  } tx[3];
  volatile struct {
    union CANMailboxTxMirRegBits mir;
    uint32_t                     mdtr;
    uint32_t                     mdlr;
    uint32_t                     mdhr;
  } rx[2];
};
static const uint8_t CAN_TX_MB_INVALID_IDX = 0xFF;

struct CANFilterRegMap {
  volatile union {
    struct {
      uint32_t finit     :1;  // 0
      uint32_t resv7_1   :7;  // 7:1
      uint32_t can2sb    :6;  // 13:8
      uint32_t resv31_14 :18; // 31:14
    };
    uint32_t dword;
  } fctlr;                           // 0x40006600
  volatile uint32_t fmcfgr;          // 0x40006604
  uint32_t          resv_0x40006608; // 0x40006608
  volatile uint32_t fscfgr;          // 0x4000660C
  uint32_t          resv_0x40006610; // 0x40006610
  volatile uint32_t fafifor;         // 0x40006614
  uint32_t          resv_0x40006618; // 0x40006618
  volatile uint32_t fwr;             // 0x4000661C
  uint32_t          resv_0x40006620; // 0x40006620
  uint32_t          resv_0x40006624; // 0x40006624
  uint32_t          resv_0x40006628; // 0x40006628
  uint32_t          resv_0x4000662C; // 0x4000662C
  uint32_t          resv_0x40006630; // 0x40006630
  uint32_t          resv_0x40006634; // 0x40006634
  uint32_t          resv_0x40006638; // 0x40006638
  uint32_t          resv_0x4000663C; // 0x4000663C
  volatile struct {
    uint32_t fr1;
    uint32_t fr2;
  } fb[28];
};

// CTRL
//-----
// TXFP
static const uint32_t CAN_CTRL_TXFP = (1 << 2);
// SLEEP[1]
// static const uint32_t CAN_CTRL_SLEEP = (1 << 1);
// INRQ[0]
static const uint32_t CAN_CTRL_INRQ = (1 << 0);

// STATR
//------
// INAK[0]
static const uint32_t CAN_STATR_INAK = (1 << 0);

// TSTATR
//-------
// CODE[25:24]
// static const uint32_t CAN_TSTATR_CODE_SHIFT = 24;

static const uint32_t CAN_TSTATR_RQCP = (1 << 0);
static const uint32_t CAN_TSTATR_TXOK = (1 << 1);
static const uint32_t CAN_TSTATR_ALST = (1 << 2);
static const uint32_t CAN_TSTATR_TERRO = (1 << 3);

// RFIFO0
//-------
static const uint32_t CAN_RFIFO0_RFOM0 = (1 << 5);
static const uint32_t CAN_RFIFO0_FOVR0 = (1 << 4);
static const uint32_t CAN_RFIFO0_FULL0 = (1 << 3);

// RFIFO1
//-------
static const uint32_t CAN_RFIFO1_RFOM1 = (1 << 5);
static const uint32_t CAN_RFIFO1_FOVR1 = (1 << 4);
static const uint32_t CAN_RFIFO1_FULL1 = (1 << 3);

// INTENR
//-------
// ERRIE
// static const uint32_t CAN_INTENR_ERRIE = (1 << 15);
// FMPIE1
static const uint32_t CAN_INTENR_FMPIE1 = (1 << 4);
// FMPIE0
static const uint32_t CAN_INTENR_FMPIE0 = (1 << 1);

// BTIMR
//------
// SILM[31]
static const uint32_t CAN_BTIMR_SILM = (1 << 31);
// LBKM[30]
static const uint32_t CAN_BTIMR_LBKM = (1 << 30);
// BRP[9:0]
static const uint32_t CAN_BTIMR_BRP_MASK = (0x3FF);

static can_rx_cb _registered_can_rx_cb;

#ifdef LIBCH32_HAS_CAN1
static struct CANRegMap __attribute__((section(".can1")))              can1;
static struct CANMailboxRegMap __attribute__((section(".can1_mb")))    can1_mb;
static struct CANFilterRegMap __attribute__((section(".can1_filter"))) can1_filter;
#endif

#ifdef LIBCH32_HAS_CAN2
static struct CANRegMap __attribute__((section(".can2")))           can2;
static struct CANMailboxRegMap __attribute__((section(".can2_mb"))) can2_mb;
#endif

static struct CANRegMap *_get_ctrl_reg_ptr(enum CanCtrlId ctrl_id) {
  struct CANRegMap *ptr = nullptr;
  switch (ctrl_id) {
    case CAN_CTRL_ID_1: {
      ptr = &can1;
      break;
    }
#if defined(LIBCH32_HAS_CAN2)
    case CAN_CTRL_ID_2: {
      ptr = &can2;
      break;
    }
#endif
    default: {
      break;
    }
  }
  return ptr;
}

static struct CANMailboxRegMap *_get_mb_ptr(enum CanCtrlId ctrl_id) {
  struct CANMailboxRegMap *ptr = nullptr;
  switch (ctrl_id) {
    case CAN_CTRL_ID_1: {
      ptr = &can1_mb;
      break;
    }
#if defined(LIBCH32_HAS_CAN2)
    case CAN_CTRL_ID_2: {
      ptr = &can2_mb;
      break;
    }
#endif
    default: {
      break;
    }
  }
  return ptr;
}

static void enbable_ctrl(struct CANRegMap *reg_ptr, uint32_t on) {
  if (reg_ptr == &can1) {
    rcc_set_peripheral_clk(RCCCan1Id, on);
    rcc_reset_peripherial(RCCCan1Id);
    return;
  }
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
  const uint32_t    ts_val = 3 + 2 + 3; // Assuming ->btimr default values

  volatile uint32_t clk_freq = rcc_get_clk_freq(RCC_CLOCK_ID_PCLK1);

  btr = clk_freq / (ts_val * bus_speed);

  tmp32 = can_ctrl->btimr & ~CAN_BTIMR_BRP_MASK;
  tmp32 |= btr - 1;

  can_ctrl->btimr = tmp32;
}

void can_init(enum CanCtrlId ctrl_id, uint32_t bus_speed, bool silent, bool loopback, can_rx_cb rx_cb) {

  struct CANRegMap *can_ctrl = _get_ctrl_reg_ptr(ctrl_id);
  if (can_ctrl) {
    _registered_can_rx_cb = rx_cb;

    enbable_ctrl(can_ctrl, 1);
    // Can controller enters SLEEP_MODE after reset, need to transition to
    // INIT_MODE
    can_ctrl->ctlr = CAN_CTRL_INRQ;
    while ((can_ctrl->statr & CAN_STATR_INAK) == 0) {};
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
}

void can_deinit(enum CanCtrlId ctrl_id) {
  struct CANRegMap *can_ctrl = _get_ctrl_reg_ptr(ctrl_id);
  if (can_ctrl) {
    enbable_ctrl(can_ctrl, 0);
  }
}

void can_filter_init(void) {

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
  uint8_t  bytes[4];
  uint32_t dword;
};

void can_filter_init_ex(enum CanCtrlId ctrl_id, const uint32_t *ids, uint32_t id_cnt) {

  uint32_t mb_start_idx;
  if (ctrl_id != CAN_CTRL_ID_1) {
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
    volatile uint32_t          id = ids[id_idx];
    volatile union CANIdFilter fr = {.dword = 0};
    volatile uint32_t          mb_idx = mb_start_idx + (id_idx / 2);

    if (id & CAN_EXT_BIT) {
      id &= 0x1fffffff;
      fr.bytes[3] = id >> 21;
      fr.bytes[2] = (id >> 13) & 0b11100000;
      fr.bytes[2] |= (id >> 13) & 0b00011111;
      fr.bytes[1] = id >> 5;
      fr.bytes[0] = ((id & 0b11111) << 3) | 0x04;
    } else {
      fr.bytes[2] = (id & 0b111) << 5;
      fr.bytes[3] = (id >> 3);
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

  struct CANRegMap *reg_ptr = _get_ctrl_reg_ptr(req->ctrl_id);
  // Find free mb
  req->_mb_idx = CAN_TX_MB_INVALID_IDX;
retry:
  do {
    if (reg_ptr->tstatr.tme0) {
      req->_mb_idx = 0;
    } else if (reg_ptr->tstatr.tme1) {
      req->_mb_idx = 1;
    } else if (reg_ptr->tstatr.tme2) {
      req->_mb_idx = 2;
    } else if (max_wait_ms--) {
      core_delay_ms(1);
      goto retry;
    } else {
      return false;
    }
  } while (req->_mb_idx == CAN_TX_MB_INVALID_IDX);

  // Get mb for this controller
  struct CANMailboxRegMap *mb = _get_mb_ptr(req->ctrl_id);

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

  struct CANRegMap *reg_ptr = _get_ctrl_reg_ptr(req->ctrl_id);

  const uint32_t tstatr = reg_ptr->tstatr.dword >> req->_mb_idx * 8;

  if ((tstatr & CAN_TSTATR_RQCP) == 0) {
    return CAN_TX_RUNNING;
  }

  enum CanTxStatus status = (tstatr & CAN_TSTATR_TXOK) ? CAN_TX_DONE : CAN_TX_ERROR;

  reg_ptr->tstatr.dword |= (CAN_TSTATR_RQCP | CAN_TSTATR_TXOK | CAN_TSTATR_ALST | CAN_TSTATR_TERRO)
                           << req->_mb_idx * 8;

  return status;
}

#ifdef LIBCH32_HAS_CAN1
static void _handle_can_rx(struct CANMailboxRegMap *mb, uint32_t rx_id) {
  CanRxMsg msg;
  union {
    struct {
      uint32_t mdlr;
      uint32_t mdhr;
    };
    uint8_t bytes[8];
  } payload;

  payload.mdlr = mb->rx[rx_id].mdlr;
  payload.mdhr = mb->rx[rx_id].mdhr;

  if (mb->rx[rx_id].mir.ide) {
    msg.id = mb->rx[rx_id].mir.exid | (mb->rx[rx_id].mir.stid << 18);
  } else {
    msg.id = mb->rx[rx_id].mir.stid;
  }

  msg.data_ptr = &payload.bytes[0];
  msg.data_len = 8;
  // msg.data_len = mb->rx[rx_id].mdtr & 0b1111;

  _registered_can_rx_cb(&msg);
}
#endif

#ifdef LIBCH32_HAS_CAN1
void USB_LP_CAN1_RX0_IRQHandler(void) NJD_IRQ_ATTRIBUTE;
void USB_LP_CAN1_RX0_IRQHandler(void) {
  if (_registered_can_rx_cb) {
    _handle_can_rx(&can1_mb, 0);
  }
  can1.rfifo0 = CAN_RFIFO0_RFOM0 | CAN_RFIFO0_FOVR0 | CAN_RFIFO0_FULL0;
}

void CAN1_RX1_IRQHandler(void) NJD_IRQ_ATTRIBUTE;
void CAN1_RX1_IRQHandler(void) {
  if (_registered_can_rx_cb) {
    _handle_can_rx(&can1_mb, 1);
  }
  can1.rfifo1 = CAN_RFIFO1_RFOM1 | CAN_RFIFO1_FOVR1 | CAN_RFIFO1_FULL1;
}
#endif

#ifdef LIBCH32_HAS_CAN2
void CAN2_RX0_IRQHandler(void) NJD_IRQ_ATTRIBUTE;
void CAN2_RX0_IRQHandler(void) {}

void CAN2_RX1_IRQHandler(void) NJD_IRQ_ATTRIBUTE;
void CAN2_RX1_IRQHandler(void) {}
#endif

#endif
