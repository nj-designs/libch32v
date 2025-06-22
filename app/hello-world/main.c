/**
 * @file main.c
 * @author Neil Johnson (nj.designs@protonmail.com)
 * @brief Hello world
 * @version 0.1
 * @date 24/05/2025
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <stdint.h>

#include "libch32v-config.h"

#include "core.h"
#include "gpio.h"
#include "printf.h"
#include "rcc.h"
#include "stdout.h"
#include "usart.h"

static struct GPIOPinSetCache ledCache;

static void print_clocks(void) {
  for (uint32_t idx = 0; idx < RCC_NUM_CLOCKS; idx++) {
    volatile uint32_t freq = rcc_get_clk_freq(rcc_all_clks[idx].id);
    if (freq < 10'000'000) {
      while (true) {
      }
    }
    printf("%s: %d\n", rcc_all_clks[idx].name, freq);
  }
}

static void setup_led(void) {
  // Setup LED
#if defined(LIBCH32_V203_FAMILY)
  const enum GPIOPinId LED_PIN = PIN_PA15;
  rcc_set_peripheral_clk(RCC_IOPA_ID, 1);
#elif defined(LIBCH32_V307_FAMILY)
  const enum GPIOPinId LED_PIN = PIN_PA10;
  rcc_set_peripheral_clk(RCC_IOPA_ID, 1);
#elif defined(LIBCH32_V003_FAMILY)
  const enum GPIOPinId LED_PIN = PIN_PC1;
  rcc_set_peripheral_clk(RCC_IOPC_ID, 1);
#else
#error Port me...
#endif
  gpio_pin_init(LED_PIN, PIN_MODE_OUTPUT_PUSH_PULL_50MHZ);
  gpio_pin_cache(LED_PIN, &ledCache);
}

/* volatile uint32_t A = 48'000'000;
volatile uint32_t B = 757; */

void main(void) {
  // volatile uint32_t mod = A % B;
  stdout_init();

  printf("Hello world!!\n");

  print_clocks();

  setup_led();

  while (1) {
    printf("On ");
    gpio_pin_set_fast(&ledCache, 1);
    core_delay_ms(500);
    printf("Off ");
    gpio_pin_set_fast(&ledCache, 0);
    core_delay_ms(500);
    // mod++;
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
    asm volatile("csrr a1, mcause");
    asm volatile("csrr a2, mepc");
    asm volatile("nop");
  }
}
