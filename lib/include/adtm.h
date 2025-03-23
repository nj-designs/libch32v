/**
 * @file adtm.h
 * @author Neil Johnson (nj.designs@protonmail.com)
 * @brief
 * @version 0.1
 * @date 2025-03-16
 *
 * @copyright Copyright (c) 2025
 *
 */
#pragma once

#include <stdint.h>

struct ADTMRegMap {
  volatile uint16_t ctlr1;  // 0x00
  uint16_t rsvd1;
  volatile uint16_t ctlr2;  // 0x04
  uint16_t rsvd2;
  volatile uint16_t smcfgr;  // 0x08
  uint16_t rsvd3;
  volatile uint16_t dmaintenr;  // 0x0C
  uint16_t rsvd4;
  volatile uint16_t intfr;  // 0x10
  uint16_t rsvd5;
  volatile uint16_t swevgr;  // 0x14
  uint16_t rsvd6;
  volatile uint16_t chctlr1;  // 0x18
  uint16_t rsvd7;
  volatile uint16_t chctlr2;  // 0x1C
  uint16_t rsvd8;
  volatile uint16_t ccer;  // 0x20
  uint16_t rsvd9;
  volatile uint16_t cnt;  // 0x24
  uint16_t rsvd10;
  volatile uint16_t psc;  // 0x28
  uint16_t rsvd11;
  volatile uint16_t atrlr;  // 0x2C
  uint16_t rsvd12;
  volatile uint16_t rptcr;  // 0x30
  uint16_t rsvd13;
  volatile uint16_t ch1cvr;  // 0x34
  uint16_t rsvd14;
  volatile uint16_t ch2cvr;  // 0x38
  uint16_t rsvd15;
  volatile uint16_t ch3cvr;  // 0x3C
  uint16_t rsvd16;
  volatile uint16_t ch4cvr;  // 0x40
  uint16_t rsvd17;
  volatile uint16_t bdtr;  // 0x44
  uint16_t rsvd18;
  volatile uint16_t dmacfgr;  // 0x48
  uint16_t rsvd19;
  volatile uint16_t dmaadr;  // 0x4C
  uint16_t rsvd20;
};

#ifdef LIBCH32_HAS_ADTM1
extern struct ADTMRegMap adtm1;
#endif

// CTRL1
//------
// CKD[9:8]
static const uint16_t ADTM_CTRL1_CKD_MASK = (0b11 << 8);
static const uint16_t ADTM_CTRL1_CKD_TDTS_TCK_INT = (0b00 << 8);
static const uint16_t ADTM_CTRL1_CKD_TDTS_2X_TCK_INT = (0b01 << 8);
static const uint16_t ADTM_CTRL1_CKD_TDTS_4X_TCK_INT = (0b10 << 8);
// ARPE[7]
static const uint16_t ADTM_CTRL1_ARPE = (1 << 7);
// CMS[6:5]
static const uint16_t ADTM_CTRL1_CMS_MASK = (0b11 << 5);
static const uint16_t ADTM_CTRL1_CMS_EDGE_ALIGN_MODE = (0b00 << 5);
static const uint16_t ADTM_CTRL1_CMS_CENTRE_ALIGN_MODE1 = (0b01 << 5);
static const uint16_t ADTM_CTRL1_CMS_CENTRE_ALIGN_MODE2 = (0b10 << 5);
static const uint16_t ADTM_CTRL1_CMS_CENTRE_ALIGN_MODE3 = (0b11 << 5);
// DIR[4]
static const uint16_t ADTM_CTRL1_DIR = (1 << 4);
// OPM[3]
static const uint16_t ADTM_CTRL1_OPM = (1 << 3);
// URS[2]
static const uint16_t ADTM_CTRL1_URS = (1 << 2);
// UDIS[1]
static const uint16_t ADTM_CTRL1_UDIS = (1 << 1);
// CEN[0]
static const uint16_t ADTM_CTRL1_CEN = (1 << 0);

// CTRL2
//------
// OIS4[14]
static const uint16_t ADTM_CTRL2_OIS4 = (1 << 14);
// OIS3N[13]
static const uint16_t ADTM_CTRL2_OIS3N = (1 << 13);
// OIS3[12]
static const uint16_t ADTM_CTRL2_OIS3 = (1 << 12);
// OIS2N[11]
static const uint16_t ADTM_CTRL2_OIS2N = (1 << 11);
// OIS2[10]
static const uint16_t ADTM_CTRL2_OIS2 = (1 << 10);
// OIS1N[9]
static const uint16_t ADTM_CTRL2_OIS1N = (1 << 9);
// OIS1[8]
static const uint16_t ADTM_CTRL2_OIS1 = (1 << 8);
// TI1S[7]
static const uint16_t ADTM_CTRL2_TI1S = (1 << 7);
// MMS[6:4]
static const uint16_t ADTM_CTRL2_MMS_MASK = (0b111 << 4);
static const uint16_t ADTM_CTRL2_MMS_RESET = (0b000 << 4);
static const uint16_t ADTM_CTRL2_MMS_ENABLE = (0b001 << 4);
static const uint16_t ADTM_CTRL2_MMS_UPDATE = (0b010 << 4);
static const uint16_t ADTM_CTRL2_MMS_COMPARE_PULSE = (0b011 << 4);
static const uint16_t ADTM_CTRL2_MMS_COMPARE_OC1REF = (0b100 << 4);
static const uint16_t ADTM_CTRL2_MMS_COMPARE_OC2REF = (0b101 << 4);
static const uint16_t ADTM_CTRL2_MMS_COMPARE_OC3REF = (0b110 << 4);
static const uint16_t ADTM_CTRL2_MMS_COMPARE_OC4REF = (0b111 << 4);
// CCDS[3]
static const uint16_t ADTM_CTRL2_CCDS = (1 << 3);
// CCUS[2]
static const uint16_t ADTM_CTRL2_CCUS = (1 << 2);
// CCPC[0]
static const uint16_t ADTM_CTRL2_CPC = (1 << 0);

// SMCFGR
//-------
// ETP[15]
static const uint16_t ADTM_SMCFGR_ETP = (1 << 15);
// ECE[14]
static const uint16_t ADTM_SMCFGR_ECE = (1 << 14);
// ETPS[13:12]
static const uint16_t ADTM_SMCFGR_ETPS_MASK = (0b11 << 13);
static const uint16_t ADTM_SMCFGR_ETPS_PRESCALE_OFF = (0b00 << 13);
static const uint16_t ADTM_SMCFGR_ETPS_PRESCALE_ERTP_DIV_2 = (0b01 << 13);
static const uint16_t ADTM_SMCFGR_ETPS_PRESCALE_ERTP_DIV_4 = (0b10 << 13);
static const uint16_t ADTM_SMCFGR_ETPS_PRESCALE_ERTP_DIV_8 = (0b11 << 13);
// ETF[11:8]
static const uint16_t ADTM_SMCFGR_ETF_MASK = (0b1111 << 8);
static const uint16_t ADTM_SMCFGR_ETF_NO_FILTER = (0b0000 << 8);
// TODO(njohn: Add remaining as needed)
// MSM[7]
static const uint16_t ADTM_SMCFGR_MSM = (1 << 7);
// TS[6:4]
static const uint16_t ADTM_SMCFGR_TS_MASK = (0b111 << 4);
static const uint16_t ADTM_SMCFGR_TS_ITR0 = (0b000 << 4);
static const uint16_t ADTM_SMCFGR_TS_ITR1 = (0b001 << 4);
static const uint16_t ADTM_SMCFGR_TS_ITR2 = (0b010 << 4);
static const uint16_t ADTM_SMCFGR_TS_ITR3 = (0b011 << 4);
static const uint16_t ADTM_SMCFGR_TS_TI1F_ED = (0b100 << 4);
static const uint16_t ADTM_SMCFGR_TS_TI1FP1 = (0b101 << 4);
static const uint16_t ADTM_SMCFGR_TS_TI2FP2 = (0b110 << 4);
static const uint16_t ADTM_SMCFGR_TS_ETRF = (0b111 << 4);
// SMS[2:0]
static const uint16_t ADTM_SMCFGR_SMS_MASK = (0b111 << 0);
static const uint16_t ADTM_SMCFGR_SMS_CK_INT = (0b000 << 0);
static const uint16_t ADTM_SMCFGR_SMS_ENCODER_MODE1 = (0b001 << 0);
static const uint16_t ADTM_SMCFGR_SMS_ENCODER_MODE2 = (0b010 << 0);
static const uint16_t ADTM_SMCFGR_SMS_ENCODER_MODE3 = (0b011 << 0);
static const uint16_t ADTM_SMCFGR_SMS_GATING_MODE = (0b101 << 0);
static const uint16_t ADTM_SMCFGR_SMS_TRIGGER_MODE = (0b110 << 0);
static const uint16_t ADTM_SMCFGR_SMS_EXT_CLK_MODE1 = (0b111 << 0);

// DMAINTENR
//----------
// TDE[14]
static const uint16_t ADTM_DMAINTENR_TDE = (1 << 14);
// COMDE[13]
static const uint16_t ADTM_DMAINTENR_COMDE = (1 << 13);
// CC4DE[12]
static const uint16_t ADTM_DMAINTENR_CC4DE = (1 << 12);
// CC3DE[11]
static const uint16_t ADTM_DMAINTENR_CC3DE = (1 << 11);
// CC2DE[10]
static const uint16_t ADTM_DMAINTENR_CC2DE = (1 << 10);
// CC1DE[9]
static const uint16_t ADTM_DMAINTENR_CC1DE = (1 << 9);
// UDE[8]
static const uint16_t ADTM_DMAINTENR_UDE = (1 << 8);
// BIE[7]
static const uint16_t ADTM_DMAINTENR_BIE = (1 << 7);
// TIE[6]
static const uint16_t ADTM_DMAINTENR_TIE = (1 << 6);
// COMIE[5]
static const uint16_t ADTM_DMAINTENR_COMIE = (1 << 5);
// CC4IE[4]
static const uint16_t ADTM_DMAINTENR_CC4IE = (1 << 4);
// CC3IE[3]
static const uint16_t ADTM_DMAINTENR_CC3IE = (1 << 3);
// CC2IE[2]
static const uint16_t ADTM_DMAINTENR_CC2IE = (1 << 2);
// CC1IE[1]
static const uint16_t ADTM_DMAINTENR_CC1IE = (1 << 1);
// UIE[0]
static const uint16_t ADTM_DMAINTENR_UIE = (1 << 0);

// INTFR
//------
// CC4OF[12]
static const uint16_t ADTM_INTFR_CC4OF = (1 << 12);
// CC3OF[11]
static const uint16_t ADTM_INTFR_CC3OF = (1 << 11);
// CC2OF[10]
static const uint16_t ADTM_INTFR_CC2OF = (1 << 10);
// CC1OF[9]
static const uint16_t ADTM_INTFR_CC1OF = (1 << 9);
// BIF[7]
static const uint16_t ADTM_INTFR_BIF = (1 << 7);
// TIF[6]
static const uint16_t ADTM_INTFR_TIF = (1 << 6);
// COMIF[5]
static const uint16_t ADTM_INTFR_COMIF = (1 << 5);
// CC4IF[4]
static const uint16_t ADTM_INTFR_CC4IF = (1 << 4);
// CC3IF[3]
static const uint16_t ADTM_INTFR_CC3IF = (1 << 3);
// CC2IF[2]
static const uint16_t ADTM_INTFR_CC2IF = (1 << 2);
// CC1IF[1]
static const uint16_t ADTM_INTFR_CC1IF = (1 << 1);
// UIF[0]
static const uint16_t ADTM_INTFR_UIF = (1 << 0);

// SWEVGR
//-------
// BG[7]
static const uint16_t ADTM_SWEVGR_BG = (1 << 7);
// TG[6]
static const uint16_t ADTM_SWEVGR_TG = (1 << 6);
// COMG[5]
static const uint16_t ADTM_SWEVGR_COMG = (1 << 5);
// CC4G[4]
static const uint16_t ADTM_SWEVGR_CC4G = (1 << 4);
// CC3G[3]
static const uint16_t ADTM_SWEVGR_CC3G = (1 << 3);
// CC2G[2]
static const uint16_t ADTM_SWEVGR_CC2G = (1 << 2);
// CC1G[1]
static const uint16_t ADTM_SWEVGR_CC1G = (1 << 1);
// UG[0]
static const uint16_t ADTM_SWEVGR_UG = (1 << 0);

// CHCTLR1
//--------
// OC2CE[15]
static const uint16_t ADTM_CHCTLR1_OC2CE = (1 << 15);
// OC2M[14:12]
static const uint16_t ADTM_CHCTLR1_OC2M_MASK = (0b111 << 12);
static const uint16_t ADTM_CHCTLR1_OC2M_FROZEN = (0b000 << 12);
static const uint16_t ADTM_CHCTLR1_OC2M_FORCED_EFFECTIVE_LEVEL = (0b001 << 12);
static const uint16_t ADTM_CHCTLR1_OC2M_SET_INACTIVE_LEVEL = (0b010 << 12);
