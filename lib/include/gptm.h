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

#include "libch32v-config.h"

/**
 * @breif Defines all possible GPTM instances
 *
 * Note: Actually availability based on specific chip
 */
enum GptmId : uint8_t {
#ifdef LIBCH32_HAS_GPTM2
  GPTM2_ID,
#endif
#ifdef LIBCH32_HAS_GPTM3
  GPTM3_ID,
#endif
#ifdef LIBCH32_HAS_GPTM4
  GPTM4_ID,
#endif
#ifdef LIBCH32_HAS_GPTM5
  GPTM5_ID,
#endif
};

enum GptmChanNum : uint8_t {
  GPTM_CHAN_1,
  GPTM_CHAN_2,
  GPTM_CHAN_3,
  GPTM_CHAN_4,
};

/**
 * @brief Setup GPTM instance for PWM
 *
 * Note: duty cycle will be set to zero
 *
 * @param gptm_id Which GPTM
 * @param pwm_freq PWM frequency
 */
void gptm_config_for_pwm(enum GptmId gptm_id, uint32_t pwm_freq);

/**
 * @brief Set PWM duty cycle
 * @param gptm_id Which GPTM
 * @param chan_id Which channel
 * @param duty_cycle Target duty cycle
 */
void gptm_set_pwm_duty(enum GptmId gptm_id, enum GptmChanNum chan_id,
                       uint32_t duty_cycle);
