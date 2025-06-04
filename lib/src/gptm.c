/**
 * @file gptm.c
 * @author Neil Johnson (nj.designs@protonmail.com)
 * @brief
 * @version 0.1
 * @date 2025-03-21
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <stdint.h>

#include "gptm.h"
#include "rcc.h"

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
static struct GPTMRegMap __attribute__((section(".gptm2"))) gptm2;
#endif

#ifdef LIBCH32_HAS_GPTM3
static struct GPTMRegMap __attribute__((section(".gptm3"))) gptm3;
#endif

#ifdef LIBCH32_HAS_GPTM4
static struct GPTMRegMap __attribute__((section(".gptm4"))) gptm4;
#endif

#ifdef LIBCH32_HAS_GPTM5
static struct GPTMRegMap __attribute__((section(".gptm5"))) gptm5;
#endif

//
// CTLR1
//------
// ARPE[7]
static const uint16_t GPTM_ARPE = (1 << 7);
// CMS[6:5]
static const uint16_t GPTM_CTRL1_CMS_EDGE_ALIGN_MODE = (0b00 << 5);
// DIR[4]
static const uint16_t GPTM_CTRL1_DIR_UP = (0 << 4);
// CEN[0]
static const uint16_t GPTM_CTRL1_CEN = (1 << 0);

// SWEVGR
//-------
static const uint16_t GPTM_SWEVGR_UG = (1 << 0);

// CHCTLR1
//--------
static const uint16_t GPTM_CHCTLn_OCcM_PWM_MODE1 = 0b110;
static const uint16_t GPTM_CHCTLR1_OC2M_PWM_MODE1 =
    (GPTM_CHCTLn_OCcM_PWM_MODE1 << 12);
// OC2PE[11]
static const uint16_t GPTM_CHCTLR1_OC2PE = (1 << 11);
// OC1M[6:4]
static const uint16_t GPTM_CHCTLR1_OC1M_PWM_MODE1 = (GPTM_CHCTLn_OCcM_PWM_MODE1 << 4);
// OC1PE[3]
static const uint16_t GPTM_CHCTLR1_OC1PE = (1 << 3);

// CHCTLR2
//--------
// OC4M[14:12]
static const uint16_t GPTM_CHCTLR2_OC4M_PWM_MODE1 =
    (GPTM_CHCTLn_OCcM_PWM_MODE1 << 12);
// OC4PE[11]
static const uint16_t GPTM_CHCTLR2_OC4PE = (1 << 11);
static const uint16_t GPTM_CHCTLR2_OC3M_PWM_MODE1 =
    (GPTM_CHCTLn_OCcM_PWM_MODE1 << 4);
// OC3PE[3]
static const uint16_t GPTM_CHCTLR2_OC3PE = (1 << 3);

// CCER
//-----
// CC4E[12]
static const uint16_t GPTM_CCER_CC4E = (1 << 12);
// CC3E[8]
static const uint16_t GPTM_CCER_CC3E = (1 << 8);
// CC2E[4]
static const uint16_t GPTM_CCER_CC2E = (1 << 4);
// CC1E[0]
static const uint16_t GPTM_CCER_CC1E = (1 << 0);

static struct GPTMRegMap* _gptm_get_reg_map(enum GptmId id) {
  struct GPTMRegMap* reg = nullptr;
  switch (id) {
#ifdef LIBCH32_HAS_GPTM2
    case GPTM2_ID: {
      reg = &gptm2;
      break;
    }
#endif
#ifdef LIBCH32_HAS_GPTM3
    case GPTM3_ID: {
      reg = &gptm3;
      break;
    }
#endif
#ifdef LIBCH32_HAS_GPTM4
    case GPTM4_ID: {
      reg = &gptm4;
      break;
    }
#endif
#ifdef LIBCH32_HAS_GPTM5
    case GPTM5_ID: {
      reg = &gptm5;
      break;
    }
#endif
    default: {
      break;
    }
  }

  return reg;
}

void gptm_config_for_pwm(enum GptmId gptm_id, uint32_t pwm_freq) {
  struct GPTMRegMap* reg = _gptm_get_reg_map(gptm_id);
  if (reg != nullptr) {
    reg->atrlr = rcc_get_clk_freq(rcc_get_clk_src(RCC_TMR2_ID)) / pwm_freq;
    reg->ctlr1 = GPTM_ARPE | GPTM_CTRL1_CMS_EDGE_ALIGN_MODE | GPTM_CTRL1_DIR_UP;
    reg->chctlr1 =
        GPTM_CHCTLR1_OC2M_PWM_MODE1 | GPTM_CHCTLR1_OC2PE | GPTM_CHCTLR1_OC1M_PWM_MODE1 | GPTM_CHCTLR1_OC1PE;
    reg->chctlr2 =
        GPTM_CHCTLR2_OC4M_PWM_MODE1 | GPTM_CHCTLR2_OC4PE | GPTM_CHCTLR2_OC3M_PWM_MODE1 | GPTM_CHCTLR2_OC3PE;
    reg->ctlr1 |= GPTM_CTRL1_CEN;
  }
}

void gptm_set_pwm_duty(enum GptmId gptm_id, enum GptmChanNum chan_id, uint32_t duty_cycle) {
  struct GPTMRegMap* reg = _gptm_get_reg_map(gptm_id);
  if (reg != nullptr) {
    if (duty_cycle > 100) {
      duty_cycle = 100;
    }
    uint32_t chcvr = 0;
    if (duty_cycle) {
      chcvr = (reg->atrlr * duty_cycle) / 100;
    }
    switch (chan_id) {
      case GPTM_CHAN_1: {
        if (chcvr) {
          reg->ch1cvr = chcvr;
          reg->ccer |= GPTM_CCER_CC1E;
        } else {
          reg->ccer &= ~(GPTM_CCER_CC1E);
        }
        break;
      }
      case GPTM_CHAN_2: {
        if (chcvr) {
          reg->ch2cvr = chcvr;
          reg->ccer |= GPTM_CCER_CC2E;
        } else {
          reg->ccer &= ~(GPTM_CCER_CC2E);
        }

        break;
      }
      case GPTM_CHAN_3: {
        if (chcvr) {
          reg->ch3cvr = chcvr;
          reg->ccer |= GPTM_CCER_CC3E;
        } else {
          reg->ccer &= ~(GPTM_CCER_CC3E);
        }

        break;
      }
      case GPTM_CHAN_4: {
        if (chcvr) {
          reg->ch4cvr = chcvr;
          reg->ccer |= GPTM_CCER_CC4E;
        } else {
          reg->ccer &= ~(GPTM_CCER_CC4E);
        }

        break;
      }
    }

    reg->swevgr = GPTM_SWEVGR_UG;
  }
}
