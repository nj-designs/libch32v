/**
 * @file rcc.c
 * @author Neil Johnson (nj.designs@protonmail.com)
 * @brief
 * @version 0.1
 * @date 2024-03-17
 *
 * @copyright Copyright (c) 2024
 *
 */
#include <stdint.h>
#include <stddef.h>

#include "rcc.h"

RCCRegMap __attribute__((section(".rcc"))) rcc;

// Must match RCCPeripheralBlockIdx
static volatile uint32_t* const clk_enable_reg_look_up[] = {
    &rcc.ahbpcenr, &rcc.apb1pcenr, &rcc.apb2pcenr};

/**
 * @brief Define values that match rcc_init()
 *
 */
static const struct RCCCfgValues initial_cfg = {.hse_freq = 0,
                                                .sysclk_freq = 8'000'000,
                                                .hclk_freq = 8'000'000,
                                                .usbclk_freq = 0,
                                                .pclk1_freq = 8'000'000,
                                                .pclk2_freq = 8'000'000,
                                                .adcclk_freq = 8'000'000,
                                                .cfgr0 = 0};

static const struct RCCCfgValues* current_cfg = &initial_cfg;

static const struct RCCCfgValues cfg_table[] = {
    {.hse_freq = 8'000'000,
     .sysclk_freq = 144'000'000,
     .hclk_freq = 144'000'000,   // RCC_CFGR0_HPRE_DIV_1
     .usbclk_freq = 48'000'000,  // RCC_CFGR0_USBPRE_DIV_3
     .pclk1_freq = 72'000'000,   // RCC_CFGR0_PPRE1_DIV_2
     .pclk2_freq = 144'000'000,  // RCC_CFGR0_PPRE2_DIV_1
     .adcclk_freq = 72'000'000,  // RCC_CFGR0_ADCPRE_DIV_2
     .cfgr0 = RCC_CFGR0_MCO_NO_CLK | RCC_CFGR0_USBPRE_DIV_3 |
              RCC_CFGR0_PLLMUL_MUL_18 | RCC_CFGR0_PLLSRC |
              RCC_CFGR0_ADCPRE_DIV_2 | RCC_CFGR0_HPRE_DIV_1 |
              RCC_CFGR0_PPRE2_DIV_1 | RCC_CFGR0_PPRE1_DIV_2},
};
static const uint16_t CFG_TABLE_SIZE = sizeof(cfg_table) / sizeof(cfg_table[0]);

void rcc_cfg_clock_tree(uint32_t hse_freq, uint32_t sysclk_freq) {
  for (uint16_t i = 0; i < CFG_TABLE_SIZE; i++) {
    const struct RCCCfgValues* ptr = &cfg_table[i];
    if (ptr->hse_freq == hse_freq && ptr->sysclk_freq == sysclk_freq) {
      // Found supported combination of hse/sysclk
      if (hse_freq) {
        rcc.ctlr |= RCC_CTLR_HSEON;
        // Wait HSE Ready
        while (1) {
          if (rcc.ctlr & RCC_CTLR_HSERDY) {
            break;
          }
        }
      }
      rcc.cfgr0 = ptr->cfgr0;
      rcc.ctlr |= RCC_CTLR_PLLON;
      while (1) {
        if (rcc.ctlr & RCC_CTLR_PLLRDY) {
          break;
        }
      }
      rcc.cfgr0 |= RCC_CFGR0_SW_PLL;
      while (1) {
        if ((rcc.cfgr0 & RCC_CFGR0_SWS_MASK) == RCC_CFGR0_SWS_PLL) {
          break;
        }
      }
      current_cfg = ptr;
    }
  }
}

void rcc_init(void) {
  rcc.ctlr |= RCC_CTLR_HSION;

  rcc.cfgr0 &= (uint32_t) ~(RCC_CFGR0_MCO_MASK | RCC_CFGR0_ADCPRE_MASK |
                            RCC_CFGR0_PPRE2_MASK | RCC_CFGR0_PPRE1_MASK |
                            RCC_CFGR0_HPRE_MASK | RCC_CFGR0_SWS_MASK |
                            RCC_CFGR0_SW_MASK);

  rcc.ctlr &= (uint32_t) ~(RCC_CTLR_PLLON | RCC_CTLR_CSSON | RCC_CTLR_HSEON);

  rcc.ctlr &= (uint32_t) ~(RCC_CTLR_HSEBYP);

  rcc.cfgr0 &= (uint32_t) ~(RCC_CFGR0_USBPRE_MASK | RCC_CFGR0_PLLMUL_MASK |
                            RCC_CFGR0_PLLXTPRE | RCC_CFGR0_PLLSRC);

  rcc.intr = RCC_INTR_CSSC | RCC_INTR_PLLRDYC | RCC_INTR_HSERDYC |
             RCC_INTR_HSIRDYC | RCC_INTR_LSERDYC | RCC_INTR_LSIRDYC;

  current_cfg = &initial_cfg;
}

void rcc_set_peripheral_clk(RCCPeripheralId id, uint32_t on) {
  volatile uint32_t* clk_enable_reg =
      clk_enable_reg_look_up[(uint32_t)id >> 16];
  if (on) {
    *clk_enable_reg |= 1 << (id & 0xFFFF);
  } else {
    *clk_enable_reg &= (uint32_t) ~(1 << (id & 0xFFFF));
  }
}

const struct RCCCfgValues* get_clk_values(void) { return current_cfg; }