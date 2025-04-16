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

  volatile uint32_t ctlr;   // 0x00
  volatile uint32_t statr;  // 0x04
  volatile uint32_t tstatr; // 0x08
  volatile uint32_t rfifo0; // 0x0C
  volatile uint32_t rfifo1; // 0x10
  volatile uint32_t intenr; // 0x14
  volatile uint32_t errsr;  // 0x18
  volatile uint32_t btimr;  // 0x1C
  volatile uint32_t ttctlr; // 0x20
  volatile uint32_t ttcnt;  // 0x24
};

struct CANMailboxRegMap {
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
};

struct CANFilterRegMap {
  volatile uint32_t fctlr;
  volatile uint32_t fmcfgr;
  volatile uint32_t fscfgr;
  volatile uint32_t fafifor;
  volatile uint32_t fwr;
  volatile uint32_t f0r1;
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
  volatile uint32_t f27r2;
};

#ifdef LIBCH32_HAS_CAN1
extern struct CANRegMap can1;
extern struct CANMailboxRegMap can1_mb;
extern struct CANFilterRegMap can1_filter;
#endif
