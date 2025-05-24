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
#include "core.h"
#include "gpio.h"
#include "printf.h"
#include "rcc.h"
#include "stdout.h"

static struct GPIOPinSetCache ledCache;

static void print_clocks(void) {
  static const struct {
    const char* str;
    enum RCCClockId id;
  } clocks[] = {

      {
          "HSE: %d\n",
          RCC_CLOCK_ID_HSE,
      },
      {
          "HSI: %d\n",
          RCC_CLOCK_ID_HSI,
      },
      {
          "SYSCLK: %d\n",
          RCC_CLOCK_ID_SYSCLK,
      },
      {
          "HCLK: %d\n",
          RCC_CLOCK_ID_HCLK,
      },
      {
          "TIM1: %d\n",
          RCC_CLOCK_ID_TIM1,
      },
      {
          "TIM2: %d\n",
          RCC_CLOCK_ID_TIM2,
      },
      {
          "PCLK1: %d\n",
          RCC_CLOCK_ID_PCLK1,
      },
      {
          "PCLK2: %d\n",
          RCC_CLOCK_ID_PCLK2,
      },

  };
  const uint32_t clk_count = sizeof(clocks) / sizeof(clocks[0]);
  for (uint32_t idx = 0; idx < clk_count; idx++) {
    volatile uint32_t freq = rcc_get_clk_freq(clocks[idx].id);
    printf(clocks[idx].str, freq);
  }
}

static void setup_led(void) {
  // Setup LED
#if defined(LIBCH32_V203_FAMILY)
  const enum GPIOPinId LED_PIN = PIN_PA15;
#elif defined(LIBCH32_V307_FAMILY)
  const enum GPIOPinId LED_PIN = PIN_PA10;
#else
#error Port me...
#endif
  rcc_set_peripheral_clk(RCC_IOPA_ID, 1);
  gpio_pin_init(LED_PIN, PIN_MODE_OUTPUT_PUSH_PULL_50MHZ);
  gpio_pin_cache(LED_PIN, &ledCache);
}

void main(void) {
  stdout_init();

  printf("Hello world!!\n");

  print_clocks();

  setup_led();

  while (1) {
    printf("On\n");
    gpio_pin_set_fast(&ledCache, 1);
    core_delay_ms(100);
    printf("Off\n");
    gpio_pin_set_fast(&ledCache, 0);
    core_delay_ms(100);
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
    asm volatile("csrr a2, mtval");
    asm volatile("nop");
  }
}
