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

#ifdef LIBCH32_HAS_GPTM2
struct GPTMRegMap __attribute__((section(".gptm2"))) gptm2;
#endif

#ifdef LIBCH32_HAS_GPTM3
struct GPTMRegMap __attribute__((section(".gptm3"))) gptm3;
#endif

#ifdef LIBCH32_HAS_GPTM4
struct GPTMRegMap __attribute__((section(".gptm4"))) gptm4;
#endif

#ifdef LIBCH32_HAS_GPTM5
struct GPTMRegMap __attribute__((section(".gptm5"))) gptm5;
#endif

#if defined(LIBCH32_V307_FAMILY)
static struct GPTMRegMap *const reg_lookup[] = {
    &gptm2, // GPTM2_ID
    &gptm3, // GPTM3_ID
    &gptm4, // GPTM4_ID
    &gptm5  // GPTM5_ID
};
#elif LIBCH32_DEVICE_ID == WCH_CH32V203G6U6
static struct GPTMRegMap *const reg_lookup[] = {
    &gptm2, // GPTM2_ID
    &gptm3, // GPTM3_ID
    &gptm4  // GPTM4_ID
};
#else
#erorr "unsupported device"
#endif

void gptm_config_for_pwm(enum GptmId gptm_id, uint32_t pwm_freq) {
  struct GPTMRegMap *reg = reg_lookup[(uint16_t)gptm_id];
  if (reg != nullptr) {
    reg->atrlr = rcc_get_clk_freq(RCC_CLOCK_ID_TIM2) / pwm_freq;
    reg->ctlr1 = GPTM_ARPE | GPTM_CTRL1_CMS_EDGE_ALIGN_MODE | GPTM_CTRL1_DIR_UP;
    reg->chctlr1 = GPTM_CHCTLR1_OC2M_PWM_MODE1 | GPTM_CHCTLR1_OC2PE |
                   GPTM_CHCTLR1_OC1M_PWM_MODE1 | GPTM_CHCTLR1_OC1PE;
    reg->chctlr2 = GPTM_CHCTLR2_OC4M_PWM_MODE1 | GPTM_CHCTLR2_OC4PE |
                   GPTM_CHCTLR2_OC3M_PWM_MODE1 | GPTM_CHCTLR2_OC3PE;
    reg->ctlr1 |= GPTM_CTRL1_CEN;
  }
}

void gptm_set_pwm_duty(enum GptmId gptm_id, enum GptmChanNum chan_id,
                       uint32_t duty_cycle) {
  struct GPTMRegMap *reg = reg_lookup[(uint16_t)gptm_id];
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
