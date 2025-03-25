/**
 * @file gptm.h
 * @author Neil Johnson (nj.designs@protonmail.com)
 * @brief
 * @version 0.1
 * @date 2025-03-21
 *
 * @copyright Copyright (c) 2025
 *
 */
#pragma once

#include <stdint.h>

struct GPTMRegMap {
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
  uint16_t rsvd22_23;
  volatile uint32_t cnt;  // 0x24
  volatile uint16_t psc;  // 0x28
  uint16_t rsvd2a_2b;
  volatile uint32_t atrlr;  // 0x2C
  uint32_t rsvd30_33;
  volatile uint32_t ch1cvr;   // 0x34
  volatile uint32_t ch2cvr;   // 0x38
  volatile uint32_t ch3cvr;   // 0x3C
  volatile uint32_t ch4cvr;   // 0x40
  uint32_t rsvd44_47;         // 0x44
  volatile uint16_t dmacfgr;  // 0x48
  uint16_t rsvd4a_4b;
  volatile uint16_t dmaadr;  // 0x4C
};

#ifdef LIBCH32_HAS_GPTM2
extern struct GPTMRegMap gptm2;
#endif

#ifdef LIBCH32_HAS_GPTM3
extern struct GPTMRegMap gptm3;
#endif

#ifdef LIBCH32_HAS_GPTM4
extern struct GPTMRegMap gptm4;
#endif
//
// CTLR1
//------
// ARPE[7]
static const uint16_t GPTM_ARPE = (1 << 7);
// CMS[6:5]
static const uint16_t GPTM_CTRL1_CMS_MASK = (0b11 << 5);
static const uint16_t GPTM_CTRL1_CMS_EDGE_ALIGN_MODE = (0b00 << 5);
static const uint16_t GPTM_CTRL1_CMS_CENTRE_ALIGN_MODE1 = (0b01 << 5);
static const uint16_t GPTM_CTRL1_CMS_CENTRE_ALIGN_MODE2 = (0b10 << 5);
static const uint16_t GPTM_CTRL1_CMS_CENTRE_ALIGN_MODE3 = (0b11 << 5);
// DIR[4]
static const uint16_t GPTM_CTRL1_DIR_UP = (0 << 4);
static const uint16_t GPTM_CTRL1_DIR_DOWN = (1 << 4);
// CEN[0]
static const uint16_t GPTM_CTRL1_CEN = (1 << 0);

// SWEVGR
//-------
static const uint16_t GPTM_SWEVGR_UG = (1 << 0);

// CHCTLR1
//--------
// OC1CE[7]
static const uint16_t GPTM_CHCTLR1_O1CE = (1 << 7);
// OC1M[6:4]
static const uint16_t GPTM_CHCTLR1_OC1M_MASK = (0b000 << 4);
static const uint16_t GPTM_CHCTLR1_OC1M_PWM_MODE1 = (0b110 << 4);
// OC1PE[3]
static const uint16_t GPTM_CHCTLR1_OC1PE = (1 << 3);
// OC1FE[2]
static const uint16_t GPTM_CHCTLR1_OC1FE = (1 << 2);

// CCER
//-----
static const uint16_t GPTM_CCER_CC1E = (1 << 0);

/**
 * @breif Defines all possible GPTM instances
 *
 * Note: Actually availability based on specific chip
 */
enum GptmId {
  GPTM2_ID,
  GPTM3_ID,
  GPTM4_ID,
};

/**
 * @brief Setup GPTM instance for PWM
 *
 * @param gptm_id Which GPTM
 */
extern void gptm_config_for_pwm(enum GptmId gptm_id);
extern void gptm_set_pwm(enum GptmId gptm_id, uint32_t duty_cycle);
