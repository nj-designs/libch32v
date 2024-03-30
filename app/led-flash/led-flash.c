/**
 * @file demo.c
 * @author Neil Johnson (nj.designs@protonmail.com)
 * @brief
 * @version 0.1
 * @date 2024-03-23
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "rcc.h"
#include "core.h"
#include "gpio.h"

#include "device_config.h"

// All interrupt handlers can be "naked" as we are using HPE
void int_handler_nmi(void) __attribute__((naked));
void int_handler_hard_fault(void) __attribute__((naked));

static GPIOPinSetCache ledCache;

static void setup_led(void) {
  // Setup LED
  rcc_set_peripheral_clk(RCC_IOPA_ID, 1);
  const GPIOPinId led_pin = PIN_PA3;
  gpio_pin_init(led_pin, PIN_MODE_OUTPUT_PUSH_PULL_50MHZ);
  gpio_pin_cache(led_pin, &ledCache);
}

void main(void) {
  rcc_cfg_clock_tree(LIBCH32_HSE_FREQ, LIBCH32_SYSCLK_FREQ);

  setup_led();

  while (1) {
    gpio_pin_set_fast(&ledCache, 1);
    core_delay_ms(1000);
    gpio_pin_set_fast(&ledCache, 0);
    core_delay_ms(1000);
  }
}

void int_handler_nmi(void) {
  while (1) {
    asm volatile("nop");
    asm volatile("nop");
  }
}

void int_handler_hard_fault(void) {
  while (1) {
    asm volatile("nop");
  }
}