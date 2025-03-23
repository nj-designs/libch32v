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

#if LIBCH32_DEVICE_ID == WCH_CH32V003F4
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
#endif

static struct GPIOPinSetCache ledCache;

#if LIBCH32_DEVICE_ID == WCH_CH32V003F4
const enum GPIOPinId LED_PIN = PIN_PD7;
#else
const enum GPIOPinId LED_PIN = PIN_PA3;
#endif

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
#if LIBCH32_DEVICE_ID == WCH_CH32V003F4
  rcc_cfg_clock_tree_ex(&ctv);
#endif
  setup_led();
  setup_pwm();

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
