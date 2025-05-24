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
#include <stddef.h>
#include <stdint.h>

#include "gpio.h"
#include "rcc.h"

/**
 * @brief
 *
 * Addr: RCC: 0x40021000
 */
typedef struct {
  volatile union {
    struct {
      uint32_t hsion    : 1;  // 0
      uint32_t hsirdy   : 1;  // 1
      uint32_t res2     : 1;  // 2
      uint32_t hsitrim  : 5;  // 7:3
      uint32_t hsical   : 8;  // 15:8
      uint32_t hseon    : 1;  // 16
      uint32_t hserdy   : 1;  // 17
      uint32_t hsebyp   : 1;  // 18
      uint32_t csson    : 1;  // 19
      uint32_t res23_20 : 4;  // 23:20
      uint32_t pllon    : 1;  // 24
      uint32_t pllrdy   : 1;  // 25
      uint32_t pll2on   : 1;  // 26
      uint32_t pll2rdy  : 1;  // 27
      uint32_t pll3on   : 1;  // 28
      uint32_t pll3rdy  : 1;  // 29
    };
    uint32_t dword;
  } ctlr;
  volatile union {
    struct {
      uint32_t sw           : 2;  // 1:0
      uint32_t sws          : 2;  // 3:2
      uint32_t hpre         : 4;  // 7:4
      uint32_t ppre1        : 3;  // 10:8
      uint32_t ppre2        : 3;  // 13:11
      uint32_t adcpre       : 2;  // 15:14
      uint32_t pllsrc       : 1;  // 16
      uint32_t pllxtpre     : 1;  // 17
      uint32_t pllmul       : 4;  // 21:18
      uint32_t usbpre       : 2;  // 23:22
      uint32_t mco          : 4;  // 27:24
      uint32_t ethpre       : 1;  // 28
      uint32_t res29        : 1;  // 29
      uint32_t adc_duty_sel : 1;  // 30
      uint32_t adcduty      : 1;  // 31
    };
    uint32_t dword;
  } cfgr0;
  volatile uint32_t intr;
  volatile uint32_t apb2prstr;
  volatile uint32_t apb1prstr;
  volatile uint32_t ahbpcenr;
  volatile uint32_t apb2pcenr;
  volatile uint32_t apb1pcenr;
  volatile uint32_t bdctlr;
  volatile uint32_t rstsckr;
  volatile uint32_t ahbrstr;
  volatile union {
    struct {
      uint32_t prediv1       : 4;  // 3:0
      uint32_t prediv2       : 4;  // 7:4
      uint32_t pll2mul       : 4;  // 11:8
      uint32_t pll3mul       : 4;  // 15:12
      uint32_t prediv1src    : 1;  // 16
      uint32_t i2s2src       : 1;  // 17
      uint32_t i2s3src       : 1;  // 18
      uint32_t rng_src       : 1;  // 19
      uint32_t eth1gsrc      : 3;  // 21:20
      uint32_t eth1g_125m_en : 1;  // 22
      uint32_t res23         : 1;  // 23
      uint32_t usbhsdiv      : 3;  // 26:24
      uint32_t usbhspllsrc   : 1;  // 27
      uint32_t usbhsclk      : 2;  // 29:28
      uint32_t usbhspll      : 1;  // 30
      uint32_t usbhssrc      : 1;  // 31
    };
    uint32_t dword;
  } cfgr2;
} RCCRegMap;

static RCCRegMap __attribute__((section(".rcc"))) rcc;

// RCC_CFGR0
//----------
// ADCPRE[15:14]
// static const uint32_t RCC_CFGR0_ADCPRE_DIV2 = 0;
// PPRE2[13:11]
static const uint32_t RCC_CFGR0_PPRE2_DIV_1  = (0b000);
static const uint32_t RCC_CFGR0_PPRE2_DIV_2  = (0b100);
static const uint32_t RCC_CFGR0_PPRE2_DIV_4  = (0b101);
static const uint32_t RCC_CFGR0_PPRE2_DIV_8  = (0b110);
static const uint32_t RCC_CFGR0_PPRE2_DIV_16 = (0b111);
// PPRE1[10:8]
static const uint32_t RCC_CFGR0_PPRE1_DIV_1  = (0b000);
static const uint32_t RCC_CFGR0_PPRE1_DIV_2  = (0b100);
static const uint32_t RCC_CFGR0_PPRE1_DIV_4  = (0b101);
static const uint32_t RCC_CFGR0_PPRE1_DIV_8  = (0b110);
static const uint32_t RCC_CFGR0_PPRE1_DIV_16 = (0b111);
// HPRE[7:4]
static const uint32_t RCC_CFGR0_HPRE_DIV_1   = (0b0000);
static const uint32_t RCC_CFGR0_HPRE_DIV_2   = (0b1000);
static const uint32_t RCC_CFGR0_HPRE_DIV_4   = (0b1001);
static const uint32_t RCC_CFGR0_HPRE_DIV_8   = (0b1010);
static const uint32_t RCC_CFGR0_HPRE_DIV_16  = (0b1011);
static const uint32_t RCC_CFGR0_HPRE_DIV_64  = (0b1100);
static const uint32_t RCC_CFGR0_HPRE_DIV_128 = (0b1101);
static const uint32_t RCC_CFGR0_HPRE_DIV_256 = (0b1110);
static const uint32_t RCC_CFGR0_HPRE_DIV_512 = (0b1111);
// SWS[3:2]/SW[1:0]
static const uint32_t RCC_CFGR0_SW_PLL = 0b10;

// If app has defined an HSE clock freq, use it
#if !defined(APP_HSE_FREQ)
#define HSE_FREQ (0)
#else
#define HSE_FREQ (APP_HSE_FREQ)
#endif

// Lookup for peripheral block enable reg
// Must match RCCPeripheralBlockIdx
static volatile uint32_t* const clk_enable_reg_look_up[] = {
    &rcc.ahbpcenr,
    &rcc.apb1pcenr,
    &rcc.apb2pcenr,
};

// Lookup for peripheral block reset reg
// Must match RCCPeripheralBlockIdx
static volatile uint32_t* const peripheral_reset_reg_look_up[] = {
    &rcc.ahbrstr,
    &rcc.apb1prstr,
    &rcc.apb2prstr,
};

static uint32_t rcc_pll_input_clk = LIBCH32_HSI_FREQ;

void rcc_init(void) {
  // Enable and wait for HSE
  if (HSE_FREQ) {
    rcc.ctlr.hseon = 1;
    while (rcc.ctlr.hserdy == 0) {
    }
    rcc.cfgr2.dword    = 0;  // PREDIV1=0 (/1)
    rcc.cfgr0.pllxtpre = 0;  //
    rcc.cfgr0.pllsrc   = 1;  // PLLSRC == HSE / 1
    rcc_pll_input_clk  = HSE_FREQ;
  }

  uint32_t pll_mul = LIBCH32_MAX_SYSCLK / rcc_pll_input_clk;
  // Configure PLL_MUL based on follow table from RM
  //
  // 0000: PLL input clock x 2; 0001: PLL input clock x 3;
  // 0010: PLL input clock x 4; 0011: PLL input clock x 5;
  // 0100: PLL input clock x 6; 0101: PLL input clock x 7;
  // 0110: PLL input clock x 8; 0111: PLL input clock x 9;
  // 1000: PLL input clock x 10; 1001: PLL input clock x 11;
  // 1010: PLL input clock x 12; 1011: PLL input clock x 13;
  // 1100: PLL input clock x 14; 1101: PLL input clock x 15;
  // 1110: PLL input clock x 16; 1111: PLL input clock x 18;
  if (pll_mul >= 18) {
    pll_mul = 17;
  } else if (pll_mul < 2) {
    pll_mul = 2;
  }
  rcc.cfgr0.pllmul = pll_mul - 2;

  rcc.cfgr0.hpre  = RCC_CFGR0_HPRE_DIV_1;
  rcc.cfgr0.ppre1 = RCC_CFGR0_PPRE1_DIV_4;
  rcc.cfgr0.ppre2 = RCC_CFGR0_PPRE2_DIV_1;

  rcc.ctlr.pllon = 1;
  while (!rcc.ctlr.pllrdy) {
  }

  rcc.cfgr0.sw = RCC_CFGR0_SW_PLL;
  while (rcc.cfgr0.sws != RCC_CFGR0_SW_PLL) {
  }

#if 0
  // Enable to use MCO
  rcc_set_peripheral_clk(RCC_IOPA_ID, 1);
  rcc_set_peripheral_clk(RCC_AFIO_ID, 1);
  gpio_pin_init(PIN_PA8, PIN_MODE_ALTERNATE_FUNC_PUSH_PULL_50MHZ);
  rcc.cfgr0.mco = 7;
#endif
}

void rcc_set_peripheral_clk(enum RCCPeripheralId id, uint32_t on) {
  volatile uint32_t* clk_enable_reg = clk_enable_reg_look_up[(uint32_t)id >> 16];
  if (on) {
    *clk_enable_reg |= 1 << (id & 0xFFFF);
  } else {
    *clk_enable_reg &= (uint32_t)~(1 << (id & 0xFFFF));
  }
}

void rcc_reset_peripherial(enum RCCPeripheralId id) {
  volatile uint32_t* clk_enable_reg = peripheral_reset_reg_look_up[(uint32_t)id >> 16];
  *clk_enable_reg                   = (id & 0xFFFF);
}

uint32_t rcc_get_clk_freq(enum RCCClockId clock_id) {
  uint32_t clock_freq = 0;

  switch (clock_id) {
    case RCC_CLOCK_ID_HSE: {
      clock_freq = HSE_FREQ;
      break;
    }
    case RCC_CLOCK_ID_HSI: {
      clock_freq = LIBCH32_HSI_FREQ;
      break;
    }
    case RCC_CLOCK_ID_SYSCLK: {
      const uint32_t pllmul = rcc.cfgr0.pllmul;
      if (pllmul == 0b1111) {
        clock_freq = rcc_pll_input_clk * 18;
      } else {
        clock_freq = rcc_pll_input_clk * (pllmul + 2);
      }
      break;
    }
    case RCC_CLOCK_ID_HCLK: {
      uint32_t sysclk = rcc_get_clk_freq(RCC_CLOCK_ID_SYSCLK);
      switch (rcc.cfgr0.hpre) {
        case RCC_CFGR0_HPRE_DIV_2: {
          clock_freq = sysclk / 2;
          break;
        }
        case RCC_CFGR0_HPRE_DIV_4: {
          clock_freq = sysclk / 4;
          break;
        }
        case RCC_CFGR0_HPRE_DIV_8: {
          clock_freq = sysclk / 8;
          break;
        }
        case RCC_CFGR0_HPRE_DIV_16: {
          clock_freq = sysclk / 16;
          break;
        }
        case RCC_CFGR0_HPRE_DIV_64: {
          clock_freq = sysclk / 64;
          break;
        }
        case RCC_CFGR0_HPRE_DIV_128: {
          clock_freq = sysclk / 128;
          break;
        }
        case RCC_CFGR0_HPRE_DIV_256: {
          clock_freq = sysclk / 256;
          break;
        }
        case RCC_CFGR0_HPRE_DIV_512: {
          clock_freq = sysclk / 512;
          break;
        }
        case RCC_CFGR0_HPRE_DIV_1: {
          clock_freq = sysclk;
          break;
        }
      };
      break;
    }
    case RCC_CLOCK_ID_TIM1: {
      clock_freq = rcc_get_clk_freq(RCC_CLOCK_ID_PCLK2);
      if (rcc.cfgr0.ppre2 != RCC_CFGR0_PPRE2_DIV_1) {
        clock_freq *= 2;
      }
      break;
    }
    case RCC_CLOCK_ID_TIM2: {
      clock_freq = rcc_get_clk_freq(RCC_CLOCK_ID_PCLK1);
      if (rcc.cfgr0.ppre1 != RCC_CFGR0_PPRE1_DIV_1) {
        clock_freq *= 2;
      }
      break;
    }
    case RCC_CLOCK_ID_PCLK1: {
      uint32_t hclk = rcc_get_clk_freq(RCC_CLOCK_ID_HCLK);

      switch (rcc.cfgr0.ppre1) {
        case RCC_CFGR0_PPRE1_DIV_2: {
          clock_freq = hclk / 2;
          break;
        }

        case RCC_CFGR0_PPRE1_DIV_4: {
          clock_freq = hclk / 4;
          break;
        }

        case RCC_CFGR0_PPRE1_DIV_8: {
          clock_freq = hclk / 8;
          break;
        }

        case RCC_CFGR0_PPRE1_DIV_16: {
          clock_freq = hclk / 16;
          break;
        }

        default: {
          clock_freq = hclk;
          break;
        }
      }

      break;
    }

    case RCC_CLOCK_ID_PCLK2: {
      uint32_t hclk = rcc_get_clk_freq(RCC_CLOCK_ID_HCLK);

      switch (rcc.cfgr0.ppre2) {
        case RCC_CFGR0_PPRE2_DIV_2: {
          clock_freq = hclk / 2;
          break;
        }

        case RCC_CFGR0_PPRE2_DIV_4: {
          clock_freq = hclk / 4;
          break;
        }

        case RCC_CFGR0_PPRE2_DIV_8: {
          clock_freq = hclk / 8;
          break;
        }

        case RCC_CFGR0_PPRE2_DIV_16: {
          clock_freq = hclk / 16;
          break;
        }

        default: {
          clock_freq = hclk;
          break;
        }
      }

      break;
    }

    default: {
      break;
    }
  }

  return clock_freq;
}
