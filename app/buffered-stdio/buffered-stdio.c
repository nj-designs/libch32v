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
#include "usart.h"

#include "device_config.h"

#include "printf.h"

void _putchar(char ch);

static GPIOPinSetCache ledCache;

static void setup_led(void) {
  // Setup LED
  rcc_set_peripheral_clk(RCC_IOPA_ID, 1);
  const GPIOPinId led_pin = PIN_PA3;
  gpio_pin_init(led_pin, PIN_MODE_OUTPUT_PUSH_PULL_50MHZ);
  gpio_pin_cache(led_pin, &ledCache);
}

void main(void) {
  printf("Hello\n");

  setup_led();

  uint32_t idx = 0;
  char ch = '0';
  while (1) {
    printf("Loop: %d\n", idx++);
    gpio_pin_set_fast(&ledCache, 1);
    core_delay_ms(100);
    gpio_pin_set_fast(&ledCache, 0);
    core_delay_ms(100);
    ch++;
    if (ch > '9') {
      ch = '0';
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
