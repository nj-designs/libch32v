/**
 * @file main.c
 * @author Neil Johnson (nj.designs@protonmail.com)
 * @brief CAN bus example
 * @version 0.1
 * @date 19/4/2025
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "can.h"
#include "core.h"
#include "gpio.h"
#include "rcc.h"

static struct GPIOPinSetCache ledCache;

const enum GPIOPinId LED_PIN = PIN_PA3;

static void setup_led(void) {
  // Setup LED
  rcc_set_peripheral_clk(RCC_IOPA_ID, 1);
  gpio_pin_init(LED_PIN, PIN_MODE_OUTPUT_PUSH_PULL_50MHZ);
  gpio_pin_cache(LED_PIN, &ledCache);
}

void setup_can(void) {

  rcc_set_peripheral_clk(RCC_AFIO_ID, 1);
  // gpio_pin_init(PIN_PA11, PIN_MODE_ALTERNATE_FUNC_PUSH_PULL_50MHZ);
  gpio_pin_init(PIN_PA11, PIN_MODE_INPUT_PULL_UP);
  gpio_pin_init(PIN_PA12, PIN_MODE_ALTERNATE_FUNC_PUSH_PULL_50MHZ);

  can_init(CAN1, 500'000);
  can_filter_init(CAN1);
}

void main(void) {
  setup_led();
  setup_can();

  can_tx(CAN1, CAN_STD_ID(0x317), 7, nullptr, true);
  while (1) {
    gpio_pin_set_fast(&ledCache, 1);
    core_delay_ms(1000);
    gpio_pin_set_fast(&ledCache, 0);
    core_delay_ms(1000);
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
