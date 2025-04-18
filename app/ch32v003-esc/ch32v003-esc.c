/**
 * @file ch32v003-esc
 * @author Neil Johnson (nj.designs@protonmail.com)
 * @brief BLDC ESC based on CH32V003 board
 * @version 0.1
 * @date 2024-03-23
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "rcc.h"
#include "core.h"
#include "gpio.h"
#include "afio.h"

static const struct RCCCfgValues ctv = {.hse_freq = 16'000'000,
                                        .sysclk_freq = 32'000'000,  // PLL is 2 * hse
                                        .hclk_freq = 32'000'000,    // RCC_CFGR0_HPRE_DIV_1
                                        .fclk = 32'000'000,
                                        .tim1 = 32'000'000,
                                        .tim2 = 32'000'000,
                                        .ahb = 32'000'000,
                                        .adcclk_freq = 16'000'000,  // RCC_CFGR0_ADCPRE_DIV_2
                                        .cfgr0 = RCC_CFGR0_MCO_NO_CLK | RCC_CFGR0_PLLSRC | RCC_CFGR0_ADCPRE_DIV_2 |
                                                 RCC_CFGR0_PPRE2_DIV_1 | RCC_CFGR0_HPRE_DIV_1 | RCC_CFGR0_SW_PLL};

static struct GPIOPinSetCache ledCache;

const enum GPIOPinId LED_PIN = PIN_PD2;

static void setup_led(void) {
  // Setup LED
  rcc_set_peripheral_clk(RCC_IOPD_ID, 1);

  afio.pcfr1 = AFIO_PCFR1_TIM1RM_PARTIAL_01;

  gpio_pin_init(LED_PIN, PIN_MODE_OUTPUT_PUSH_PULL_50MHZ);
  gpio_pin_cache(LED_PIN, &ledCache);
}

void main(void) {
  rcc_cfg_clock_tree_ex(&ctv);

  setup_led();

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
