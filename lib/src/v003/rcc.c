/**
 * @file v003/rcc.c
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
    };
    uint32_t dword;
  } ctlr;
  volatile union {
    struct {
      uint32_t sw       : 2;  // 1:0
      uint32_t sws      : 2;  // 3:2
      uint32_t hpre     : 4;  // 7:4
      uint32_t res10_8  : 3;  // 10:8
      uint32_t adcpre   : 5;  // 15:11
      uint32_t pllsrc   : 1;  // 16
      uint32_t res23_17 : 7;  // 23:17
      uint32_t mco      : 7;  // 26:24
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
} RCCRegMap;

static RCCRegMap __attribute__((section(".rcc"))) rcc;

// RCC_CFGR0
//----------
// ADCPRE[15:14]
static const uint32_t RCC_CFGR0_ADCPRE_DIV2 = 0;
// PPRE2[13:11]
// static const uint32_t RCC_CFGR0_PPRE2_DIV_1 = (0b000);
// static const uint32_t RCC_CFGR0_PPRE2_DIV_2  = (0b100);
// static const uint32_t RCC_CFGR0_PPRE2_DIV_4  = (0b101);
// static const uint32_t RCC_CFGR0_PPRE2_DIV_8  = (0b110);
// static const uint32_t RCC_CFGR0_PPRE2_DIV_16 = (0b111);
// PPRE1[10:8]
// static const uint32_t RCC_CFGR0_PPRE1_DIV_1 = (0b000);
// static const uint32_t RCC_CFGR0_PPRE1_DIV_2 = (0b100);
// static const uint32_t RCC_CFGR0_PPRE1_DIV_4 = (0b101);
// static const uint32_t RCC_CFGR0_PPRE1_DIV_8 = (0b110);
// HPRE[7:4]
static const uint32_t RCC_CFGR0_HPRE_DIV_1 = (0b0000);
static const uint32_t RCC_CFGR0_HPRE_DIV_2 = (0b0001);
// static const uint32_t RCC_CFGR0_HPRE_DIV_3   = (0b0010);
static const uint32_t RCC_CFGR0_HPRE_DIV_4 = (0b0011);
// static const uint32_t RCC_CFGR0_HPRE_DIV_5   = (0b0100);
// static const uint32_t RCC_CFGR0_HPRE_DIV_6   = (0b0101);
// static const uint32_t RCC_CFGR0_HPRE_DIV_7   = (0b0110);
static const uint32_t RCC_CFGR0_HPRE_DIV_8   = (0b0111);
static const uint32_t RCC_CFGR0_HPRE_DIV_16  = (0b1011);
static const uint32_t RCC_CFGR0_HPRE_DIV_32  = (0b1100);
static const uint32_t RCC_CFGR0_HPRE_DIV_64  = (0b1101);
static const uint32_t RCC_CFGR0_HPRE_DIV_128 = (0b1110);
static const uint32_t RCC_CFGR0_HPRE_DIV_256 = (0b1111);
// SWS[3:2]/SW[1:0]
static const uint32_t RCC_CFGR0_SW_HSI = 0b00;
static const uint32_t RCC_CFGR0_SW_HSE = 0b01;
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
    nullptr,
    &rcc.apb1prstr,
    &rcc.apb2prstr,
};

static uint32_t rcc_pll_input_clk = LIBCH32_HSI_FREQ / 2;

void rcc_init(void) {
  // Enable and wait for HSE
  if (HSE_FREQ) {
    rcc.ctlr.hseon = 1;
    while (rcc.ctlr.hserdy == 0) {
    }
    rcc.cfgr0.pllsrc  = 1;  // PLLSRC == PREDIV1
    rcc_pll_input_clk = HSE_FREQ;
  }

  rcc.cfgr0.hpre   = RCC_CFGR0_HPRE_DIV_1;
  rcc.cfgr0.adcpre = RCC_CFGR0_ADCPRE_DIV2;

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
  if (clk_enable_reg != nullptr) {
    *clk_enable_reg = (id & 0xFFFF);
  }
}

uint32_t rcc_get_clk_freq(enum RCCClockId clock_id) {
  volatile uint32_t clock_freq = 0;

  switch (clock_id) {
    case RCC_CLOCK_ID_HSE: {
      clock_freq = HSE_FREQ;
      break;
    }
    case RCC_CLOCK_ID_HSI: {
      clock_freq = LIBCH32_HSI_FREQ;
      break;
    }
    case RCC_CLOCK_ID_PLL: {
      if (rcc.cfgr0.pllsrc) {
        clock_freq = rcc_get_clk_freq(RCC_CLOCK_ID_HSE);
      } else {
        clock_freq = rcc_get_clk_freq(RCC_CLOCK_ID_HSI);
      }
      clock_freq *= 2;
      break;
    }
    case RCC_CLOCK_ID_SYSCLK: {
      switch (rcc.cfgr0.sws) {
        case RCC_CFGR0_SW_HSI: {
          clock_freq = rcc_get_clk_freq(RCC_CLOCK_ID_HSI);
          break;
        }
        case RCC_CFGR0_SW_HSE: {
          clock_freq = rcc_get_clk_freq(RCC_CLOCK_ID_HSE);
          break;
        }
        case RCC_CFGR0_SW_PLL: {
          clock_freq = rcc_get_clk_freq(RCC_CLOCK_ID_PLL);
          break;
        }
        default: {
          // TODO(njohn) - Handle?
          while (true) {
          }
          break;
        }
      }
      break;
    }
    case RCC_CLOCK_ID_HCLK: {
      uint32_t sysclk = rcc_get_clk_freq(RCC_CLOCK_ID_SYSCLK);
      switch (rcc.cfgr0.hpre) {
        case RCC_CFGR0_HPRE_DIV_1: {
          clock_freq = sysclk;
          break;
        }
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
        case RCC_CFGR0_HPRE_DIV_32: {
          clock_freq = sysclk / 32;
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
        default: {
          // TODO(njohn) - What to do here?
          break;
        }
      };
      break;
    }

    case RCC_CLOCK_ID_ADCPRE: {
      clock_freq = rcc_get_clk_freq(RCC_CLOCK_ID_HCLK) / 2;
      break;
    }

    default: {
      break;
    }
  }

  return clock_freq;
}

enum RCCClockId rcc_get_clk_src(enum RCCPeripheralId pid) {
  enum RCCClockId cid = RCC_CLOCK_ID_UNKOWN;

  switch (pid) {
    case RCC_SRAM_ID:
    case RCC_DMA1_ID:
    case RCC_I2C1_ID:
    case RCC_TMR2_ID:
    case RCC_USART1_ID:
    case RCC_SPI1_ID:
    case RCC_TMR1_ID:
    case RCC_IOPD_ID:
    case RCC_IOPC_ID:
    case RCC_IOPA_ID:
    case RCC_AFIO_ID: {
      cid = RCC_CLOCK_ID_HCLK;
      break;
    }
    case RCC_ADC1_ID: {
      cid = RCC_CLOCK_ID_ADCPRE;
      break;
    }
    default: {
      break;
    }
  }

  return cid;
}
