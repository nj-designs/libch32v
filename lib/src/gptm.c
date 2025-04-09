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

#if LIBCH32_DEVICE_ID == WCH_CH32V203G6U6
static struct GPTMRegMap* const reg_lookup[] = {
    &gptm2,  // GPTM2_ID
    &gptm3,  // GPTM3_ID
    &gptm4   // GPTM4_ID
};
#else
#erorr "unsupported device"
#endif

void gptm_config_for_pwm(enum GptmId gptm_id, enum GptmChanNum chan_id, uint32_t pwm_freq) {
  struct GPTMRegMap* reg = reg_lookup[(uint16_t)gptm_id];
  if (reg != nullptr) {
    gptm2.ctlr1 = GPTM_ARPE | GPTM_CTRL1_CMS_EDGE_ALIGN_MODE | GPTM_CTRL1_DIR_DOWN;
    gptm2.atrlr = rcc_get_clk_freq(RCC_CLOCK_ID_TIM2) / pwm_freq;
    gptm2.chctlr1 = GPTM_CHCTLR1_OC1M_PWM_MODE1 | GPTM_CHCTLR1_OC1PE;
    gptm2.ccer = GPTM_CCER_CC1E;
    gptm2.ch1cvr = 900;
    gptm2.swevgr = GPTM_SWEVGR_UG;
    gptm2.ctlr1 |= GPTM_CTRL1_CEN;
  }
}

void gptm_set_pwm(enum GptmId gptm_id, uint32_t duty_cycle) {
  (void)duty_cycle;

  struct GPTMRegMap* reg = reg_lookup[(uint16_t)gptm_id];
  if (reg != nullptr) {
    volatile uint32_t v = reg->atrlr / duty_cycle;
    (void)v;
  }
}

void gptm_cfg_pwm(enum GptmId gptm_id, enum GptmChanNum chan_id, uint32_t pwm_freq, uint32_t initial_duty_cycle) {
  struct GPTMRegMap* reg = reg_lookup[(uint16_t)gptm_id];
  if (reg != nullptr) {
    volatile uint32_t v = reg->atrlr / duty_cycle;
    (void)v;
  }
}
