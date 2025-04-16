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

#include "core.h"
#include "gpio.h"
#include "gptm.h"
#include "rcc.h"

static struct GPIOPinSetCache ledCache;

static const enum GPIOPinId LED_PIN = PIN_PA3;

static void setup_led(void) {
  rcc_set_peripheral_clk(RCC_IOPA_ID, 1);
  gpio_pin_init(LED_PIN, PIN_MODE_OUTPUT_PUSH_PULL_50MHZ);
  gpio_pin_cache(LED_PIN, &ledCache);
}

static void setup_pwm(void) {
  rcc_set_peripheral_clk(RCC_TIM2EN_ID, 1);
  rcc_set_peripheral_clk(RCC_AFIO_ID, 1);
  gpio_pin_init(PIN_PA0, PIN_MODE_ALTERNATE_FUNC_PUSH_PULL_50MHZ);
  gpio_pin_init(PIN_PA1, PIN_MODE_ALTERNATE_FUNC_PUSH_PULL_50MHZ);
  gptm_config_for_pwm(GPTM2_ID, 30'000);
}

void main(void) {
  setup_led();
  setup_pwm();

  while (1) {
    for (uint32_t pwm = 0; pwm <= 100; pwm++) {
      gptm_set_pwm_duty(GPTM2_ID, GPTM_CHAN_1, pwm);
      gptm_set_pwm_duty(GPTM2_ID, GPTM_CHAN_2, 100 - pwm);
      gpio_pin_set_fast(&ledCache, 1);
      core_delay_ms(200);
      gpio_pin_set_fast(&ledCache, 0);
      core_delay_ms(200);
    }
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
