/**
 * @file can.h
 * @author Neil Johnson (nj.designs@protonmail.com)
 * @brief
 * @version 0.1
 * @date 2025-04-16
 *
 * @copyright Copyright (c) 2025
 *
 */
#pragma once

#include <stdint.h>

struct CANRegMap {
  volatile uint32_t ctlr;  // 0x00
  volatile uint32_t statr; // 0x04
  volatile union {
    struct {
      uint32_t rqcp0 : 1;    // 0
      uint32_t txok0 : 1;    // 1
      uint32_t alst0 : 1;    // 2
      uint32_t terr0 : 1;    // 3
      uint32_t res6_4 : 3;   // 6:4
      uint32_t abrq0 : 1;    // 7
      uint32_t rqcp1 : 1;    // 8
      uint32_t txok1 : 1;    // 9
      uint32_t alst1 : 1;    // 10
      uint32_t terr1 : 1;    // 11
      uint32_t res14_12 : 3; // 14:12
      uint32_t abrq1 : 1;    // 15
      uint32_t rqcp2 : 1;    // 16
      uint32_t tkok2 : 1;    // 17
      uint32_t alst2 : 1;    // 18
      uint32_t terr2 : 1;    // 19
      uint32_t res22_20 : 3; // 22:20
      uint32_t abrq2 : 1;    // 23
      uint32_t code : 2;     // 25:24
      uint32_t tme0 : 1;     // 26
      uint32_t tme1 : 1;     // 27
      uint32_t tme2 : 1;     // 28
      uint32_t low0 : 1;     // 29
      uint32_t low1 : 1;     // 30
      uint32_t low2 : 1;     // 31
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
    uint32_t txrq : 1;  // 0
    uint32_t rtr : 1;   // 1
    uint32_t ide : 1;   // 2
    uint32_t exid : 18; // 20:3
    uint32_t stid : 11; // 31:21
  };
  uint32_t dword;
};

struct CANMailboxRegMap {
  volatile struct {
    union CANMailboxTxMirRegBits mir;
    uint32_t mdtr;
    uint32_t mdlr;
    uint32_t mdhr;
  } tx[3];
  volatile struct {
    uint32_t mir;
    uint32_t mdtr;
    uint32_t mdlr;
    uint32_t mdhr;
  } rx[2];
};
static const uint8_t CAN_TX_MB_INVALID_IDX = 0xFF;

/* struct CANMailboxRegMap {
  // Tx Mailbox0
  volatile uint32_t txmir0;
  volatile uint32_t txmdtr0;
  volatile uint32_t txmdlr0;
  volatile uint32_t txmdhr0;
  // Tx Mailbox1
  volatile uint32_t txmir1;
  volatile uint32_t txmdtr1;
  volatile uint32_t txmdlr1;
  volatile uint32_t txmdhr1;
  // Tx Mailbox2
  volatile uint32_t txmir2;
  volatile uint32_t txmdtr2;
  volatile uint32_t txmdlr2;
  volatile uint32_t txmdhr2;
  // Rx Mailbox 0
  volatile uint32_t rxmir0;
  volatile uint32_t rxmdtr0;
  volatile uint32_t rxmdlr0;
  volatile uint32_t rxmdhr0;
  // Rx Mailbox 1
  volatile uint32_t rxmir1;
  volatile uint32_t rxmdtr1;
  volatile uint32_t rxmdlr1;
  volatile uint32_t rxmdhr1;
}; */

struct CANFilterRegMap {
  volatile union {
    struct {
      uint32_t finit : 1;      // 0
      uint32_t resv7_1 : 7;    // 7:1
      uint32_t can2sh : 6;     // 13:8
      uint32_t resv31_14 : 18; // 31:14
    };
    uint32_t dword;
  } fctlr;                   // 0x40006600
  volatile uint32_t fmcfgr;  // 0x40006604
  uint32_t resv_0x40006608;  // 0x40006608
  volatile uint32_t fscfgr;  // 0x4000660C
  uint32_t resv_0x40006610;  // 0x40006610
  volatile uint32_t fafifor; // 0x40006614
  uint32_t resv_0x40006618;  // 0x40006618
  volatile uint32_t fwr;     // 0x4000661C
  uint32_t resv_0x40006620;  // 0x40006620
  uint32_t resv_0x40006624;  // 0x40006624
  uint32_t resv_0x40006628;  // 0x40006628
  uint32_t resv_0x4000662C;  // 0x4000662C
  uint32_t resv_0x40006630;  // 0x40006630
  uint32_t resv_0x40006634;  // 0x40006634
  uint32_t resv_0x40006638;  // 0x40006638
  uint32_t resv_0x4000663C;  // 0x4000663C
  volatile struct {
    uint32_t fr1;
    uint32_t fr2;
  } fb[28];
  /**
  volatile uint32_t f0r1;    // 0x40006640
  volatile uint32_t f0r2;
  volatile uint32_t f1r1;
  volatile uint32_t f1r2;
  volatile uint32_t f2r1;
  volatile uint32_t f2r2;
  volatile uint32_t f3r1;
  volatile uint32_t f3r2;
  volatile uint32_t f4r1;
  volatile uint32_t f4r2;
  volatile uint32_t f5r1;
  volatile uint32_t f5r2;
  volatile uint32_t f6r1;
  volatile uint32_t f6r2;
  volatile uint32_t f7r1;
  volatile uint32_t f7r2;
  volatile uint32_t f8r1;
  volatile uint32_t f8r2;
  volatile uint32_t f9r1;
  volatile uint32_t f9r2;
  volatile uint32_t f10r1;
  volatile uint32_t f10r2;
  volatile uint32_t f11r1;
  volatile uint32_t f11r2;
  volatile uint32_t f12r1;
  volatile uint32_t f12r2;
  volatile uint32_t f13r1;
  volatile uint32_t f13r2;
  volatile uint32_t f14r1;
  volatile uint32_t f14r2;
  volatile uint32_t f15r1;
  volatile uint32_t f15r2;
  volatile uint32_t f16r1;
  volatile uint32_t f16r2;
  volatile uint32_t f17r1;
  volatile uint32_t f17r2;
  volatile uint32_t f18r1;
  volatile uint32_t f18r2;
  volatile uint32_t f19r1;
  volatile uint32_t f19r2;
  volatile uint32_t f20r1;
  volatile uint32_t f20r2;
  volatile uint32_t f21r1;
  volatile uint32_t f21r2;
  volatile uint32_t f22r1;
  volatile uint32_t f22r2;
  volatile uint32_t f23r1;
  volatile uint32_t f23r2;
  volatile uint32_t f24r1;
  volatile uint32_t f24r2;
  volatile uint32_t f25r1;
  volatile uint32_t f25r2;
  volatile uint32_t f26r1;
  volatile uint32_t f26r2;
  volatile uint32_t f27r1;
  volatile uint32_t f27r2; */
};

#ifdef LIBCH32_HAS_CAN1
extern struct CANRegMap can1;
#define CAN1 (&can1)
extern struct CANMailboxRegMap can1_mb;
extern struct CANFilterRegMap can1_filter;
#endif

#ifdef LIBCH32_HAS_CAN2
extern struct CANRegMap can2;
#define CAN2 (&can2)
extern struct CANMailboxRegMap can2_mb;
#endif

// CTRL
//-----
// SLEEP[0]
static const uint32_t CAN_CTRL_SLEEP = (1 << 1);
// INRQ[0]
static const uint32_t CAN_CTRL_INRQ = (1 << 0);

// STATR
//------
// INAK[0]
static const uint32_t CAN_STATR_INAK = (1 << 0);

// TSTATR
//-------
// CODE[25:24]
static const uint32_t CAN_TSTATR_CODE_SHIFT = 24;
static const uint32_t CAN_TSTATR_RQCP = (1 << 0);
static const uint32_t CAN_TSTATR_TXOK = (1 << 1);
static const uint32_t CAN_TSTATR_TERRO = (1 << 3);
// BTIMR
//------
// SILM[31]
static const uint32_t CAN_BTIMR_SILM = (1 << 31);
// LBKM[30]
static const uint32_t CAN_BTIMR_LBKM = (1 << 30);
// BRP[9:0]
static const uint32_t CAN_BTIMR_BRP_MASK = (0x3FF);

void can_init(struct CANRegMap *reg_ptr, uint32_t bus_speed);

void can_filter_init(struct CANRegMap *reg_ptr);

#define CAN_EXT_BIT ((uint32_t)(1 << 31))
#define CAN_STD_ID(_id) ((uint32_t)(_id & 0x7ff))
#define CAN_EXT_ID(_id) (CAN_EXT_BIT | (uint32_t)(_id & 0x1fffffff))

void can_tx(struct CANRegMap *reg_ptr, uint32_t id, uint32_t data_len,
            const uint8_t *data_ptr, bool block);

struct CANTxReq {
  struct CANRegMap *reg_ptr;
  uint32_t id;
  const uint8_t *data_ptr;
  uint8_t data_len;
  // Private
  uint8_t _mb_idx;
};

bool can_tx_req(struct CANTxReq *req);

enum CanTxStatus {

  CAN_TX_DONE,
  CAN_TX_ERROR,
  CAN_TX_RUNNING
};
enum CanTxStatus can_check_tx_complete(const struct CANTxReq *req);
