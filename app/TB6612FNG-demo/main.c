/**
 * @file main.c
 * @author Neil Johnson (nj.designs@protonmail.com)
 * @brief
 * @version 0.1
 * @date 2025-03-20
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "rcc.h"
#include "core.h"
#include "gpio.h"
#include "gptm.h"

static struct GPIOPinSetCache ledCache;

const enum GPIOPinId LED_PIN = PIN_PA3;

static void setup_led(void) {
  // Setup LED
  rcc_set_peripheral_clk(RCC_IOPA_ID, 1);
  gpio_pin_init(LED_PIN, PIN_MODE_OUTPUT_PUSH_PULL_50MHZ);
  gpio_pin_cache(LED_PIN, &ledCache);
}

static void setup_pwm(void) {
  // Setup PA0 for 20Khz PWM
  rcc_set_peripheral_clk(RCC_TIM2EN_ID, 1);
  rcc_set_peripheral_clk(RCC_AFIO_ID, 1);
  gpio_pin_init(PIN_PA0, PIN_MODE_ALTERNATE_FUNC_PUSH_PULL_50MHZ);
  gptm2.ctlr1 = GPTM_ARPE | GPTM_CTRL1_CMS_EDGE_ALIGN_MODE | GPTM_CTRL1_DIR_DOWN;
  gptm2.atrlr = 3600;
  gptm2.chctlr1 = GPTM_CHCTLR1_OC1M_PWM_MODE1 | GPTM_CHCTLR1_OC1PE;
  gptm2.ccer = GPTM_CCER_CC1E;
  gptm2.ch1cvr = 900;
  gptm2.swevgr = GPTM_SWEVGR_UG;
  gptm2.ctlr1 |= GPTM_CTRL1_CEN;
}

void main(void) {
  setup_led();
  setup_pwm();

  gptm_set_pwm(GPTM2_ID, 900);
  while (1) {
    gpio_pin_set_fast(&ledCache, 1);
    core_delay_ms(200);
    gpio_pin_set_fast(&ledCache, 0);
    core_delay_ms(200);
  }
}

void int_handler_nmi(void) NJD_IRQ_ATTRIBUTE;
void int_handler_nmi(void) {
  while (1) {
    asm volatile("nop");
    asm volatile("nop");
  }
}

void int_handler_hard_fault(void) NJD_IRQ_ATTRIBUTE;
void int_handler_hard_fault(void) {
  while (1) {
    asm volatile("nop");
  }
}
