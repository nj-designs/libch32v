/**
 * @file rcc.h
 * @author Neil Johnson (nj.designs@protonmail.com)
 * @brief
 * @version 0.1
 * @date 2024-03-17
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once

#include <stdint.h>

/**
 * @brief
 *
 * Addr: RCC: 0x40021000
 */
typedef struct {
  volatile uint32_t ctlr;
  volatile uint32_t cfgr0;
  volatile uint32_t intr;
  volatile uint32_t apb2prstr;
  volatile uint32_t apb1prstr;
  volatile uint32_t ahbpcenr;
  volatile uint32_t apb2pcenr;
  volatile uint32_t apb1pcenr;
  volatile uint32_t reserved0;
  volatile uint32_t rstsckr;
  volatile uint32_t ahbrstr;
  volatile uint32_t cfgr2;
} RCCRegMap;

extern RCCRegMap rcc;

// RCC_CTRL
//---------
// PLLRDY[25]
static const uint32_t RCC_CTLR_PLLRDY = (1 << 25);
// PLLON[24]
static const uint32_t RCC_CTLR_PLLON = (1 << 24);
// CSSON[19]
static const uint32_t RCC_CTLR_CSSON = (1 << 19);
// HSEBYP[18]
static const uint32_t RCC_CTLR_HSEBYP = (1 << 18);
// HSERDY[17]
static const uint32_t RCC_CTLR_HSERDY = (1 << 17);
// HSEON[16]
static const uint32_t RCC_CTLR_HSEON = (1 << 16);
// HSION[0]
static const uint32_t RCC_CTLR_HSION = (1 << 0);

// RCC_CFGR0
//----------
// ADCDUTY[31]
// static const uint32_t RCC_CFGR0_ADCDUTY = (1 << 31);
// MCO[27:24]
static const uint32_t RCC_CFGR0_MCO_MASK = (0b1111 << 24);
static const uint32_t RCC_CFGR0_MCO_NO_CLK = (0b0000 << 24);
// USBPRE[23:22]
static const uint32_t RCC_CFGR0_USBPRE_MASK = (0b11 << 22);
// static const uint32_t RCC_CFGR0_USBPRE_DIV_1 = (0b00 << 22);
// static const uint32_t RCC_CFGR0_USBPRE_DIV_2 = (0b01 << 22);
static const uint32_t RCC_CFGR0_USBPRE_DIV_3 = (0b10 << 22);
// static const uint32_t RCC_CFGR0_USBPRE_DIV_5 = (0b11 << 22);
// PLLMUL[21:18]
static const uint32_t RCC_CFGR0_PLLMUL_MASK = (0b1111 << 18);
static const uint32_t RCC_CFGR0_PLLMUL_MUL_18 = (0b1111 << 18);
// PLLXTPRE[17]
static const uint32_t RCC_CFGR0_PLLXTPRE = (1 << 17);
// PLLSRC[16]
static const uint32_t RCC_CFGR0_PLLSRC = (1 << 16);
// ADCPRE[15:14]
static const uint32_t RCC_CFGR0_ADCPRE_MASK = (0b11 << 14);
static const uint32_t RCC_CFGR0_ADCPRE_DIV_2 = (0b00 << 14);
// PPRE2[13:11]
static const uint32_t RCC_CFGR0_PPRE2_MASK = (0b111 << 11);
static const uint32_t RCC_CFGR0_PPRE2_DIV_1 = (0b000 << 11);
// PPRE1[10:8]
static const uint32_t RCC_CFGR0_PPRE1_MASK = (0b111 << 8);
static const uint32_t RCC_CFGR0_PPRE1_DIV_2 = (0b100 << 8);
// HPRE[7:4]
static const uint32_t RCC_CFGR0_HPRE_MASK = (0b1111 << 4);
static const uint32_t RCC_CFGR0_HPRE_DIV_1 = (0b0000 << 4);
// SWS[3:2]
static const uint32_t RCC_CFGR0_SWS_MASK = (0b11 << 2);
static const uint32_t RCC_CFGR0_SWS_PLL = (0b10 << 2);
// SWS[1:0]
static const uint32_t RCC_CFGR0_SW_MASK = (0b11);
static const uint32_t RCC_CFGR0_SW_PLL = (0b10);

// RCC_INTR
//---------
// CSSC[17]
static const uint32_t RCC_INTR_CSSC = (1 << 23);
// PLLRDYC[20]
static const uint32_t RCC_INTR_PLLRDYC = (1 << 20);
// HSERDYC[19]
static const uint32_t RCC_INTR_HSERDYC = (1 << 19);
// HSIRDYC[18]
static const uint32_t RCC_INTR_HSIRDYC = (1 << 18);
// LSERDYC[17]
static const uint32_t RCC_INTR_LSERDYC = (1 << 17);
// LSIRDYC[16]
static const uint32_t RCC_INTR_LSIRDYC = (1 << 16);

#define CREATE_RCC_PERIPHERAL_ID(RCC_IDX, PIN_IDX) ((RCC_IDX) << 16 | PIN_IDX)

typedef enum {
  RCC_AHB_IDX,
  RCC_PB1_IDX,
  RCC_PB2_IDX,
} RCCPeripheralBlockIdx;

/**
 * @brief Define RCCperipherals for HB, PB2 & PB1
 *        Used in both clk & reset context but not all
 *        entries are valid for both use cases
 */
typedef enum {
  // HB

  RCCBlesId = CREATE_RCC_PERIPHERAL_ID(RCC_AHB_IDX, 17),
  RCCBlecId = CREATE_RCC_PERIPHERAL_ID(RCC_AHB_IDX, 16),
  RCCEthMacRId = CREATE_RCC_PERIPHERAL_ID(RCC_AHB_IDX, 16),
  RCCEthMacTId = CREATE_RCC_PERIPHERAL_ID(RCC_AHB_IDX, 15),
  RCCEthMacId = CREATE_RCC_PERIPHERAL_ID(RCC_AHB_IDX, 14),
  RCCDvpId = CREATE_RCC_PERIPHERAL_ID(RCC_AHB_IDX, 13),
  RCCOtgFsId = CREATE_RCC_PERIPHERAL_ID(RCC_AHB_IDX, 12),
  RCCUsbHsId = CREATE_RCC_PERIPHERAL_ID(RCC_AHB_IDX, 11),
  RCCSdioId = CREATE_RCC_PERIPHERAL_ID(RCC_AHB_IDX, 10),
  RCCRngId = CREATE_RCC_PERIPHERAL_ID(RCC_AHB_IDX, 9),
  RCCFsmcenId = CREATE_RCC_PERIPHERAL_ID(RCC_AHB_IDX, 8),
  RCCrcId = CREATE_RCC_PERIPHERAL_ID(RCC_AHB_IDX, 6),
  RCSramIfId = CREATE_RCC_PERIPHERAL_ID(RCC_AHB_IDX, 2),
  RCDma2Id = CREATE_RCC_PERIPHERAL_ID(RCC_AHB_IDX, 1),
  RC_DMA1_ID = CREATE_RCC_PERIPHERAL_ID(RCC_AHB_IDX, 0),

  // PB2
  RCCTimer10Id = CREATE_RCC_PERIPHERAL_ID(RCC_PB2_IDX, 20),
  RCCTimer9Id = CREATE_RCC_PERIPHERAL_ID(RCC_PB2_IDX, 19),
  RCC_USART1_ID = CREATE_RCC_PERIPHERAL_ID(RCC_PB2_IDX, 14),
  RCCTimer81Id = CREATE_RCC_PERIPHERAL_ID(RCC_PB2_IDX, 13),
  RCC_SPI1_ID = CREATE_RCC_PERIPHERAL_ID(RCC_PB2_IDX, 12),
  RCCTimer1Id = CREATE_RCC_PERIPHERAL_ID(RCC_PB2_IDX, 11),
  RCCAdc2Id = CREATE_RCC_PERIPHERAL_ID(RCC_PB2_IDX, 10),
  RCCAdc1Id = CREATE_RCC_PERIPHERAL_ID(RCC_PB2_IDX, 9),
  RCCIoPortEId = CREATE_RCC_PERIPHERAL_ID(RCC_PB2_IDX, 6),
  RCCIoPortDId = CREATE_RCC_PERIPHERAL_ID(RCC_PB2_IDX, 5),
  RCCIoPortCId = CREATE_RCC_PERIPHERAL_ID(RCC_PB2_IDX, 4),
  RCCIoPortBId = CREATE_RCC_PERIPHERAL_ID(RCC_PB2_IDX, 3),
  RCC_IOPA_ID = CREATE_RCC_PERIPHERAL_ID(RCC_PB2_IDX, 2),
  RCC_AFIO_ID = CREATE_RCC_PERIPHERAL_ID(RCC_PB2_IDX, 0),

  // PB1
  RCCDacId = CREATE_RCC_PERIPHERAL_ID(RCC_PB1_IDX, 29),
  RCCPwrIf5Id = CREATE_RCC_PERIPHERAL_ID(RCC_PB1_IDX, 28),
  RCCBkpId = CREATE_RCC_PERIPHERAL_ID(RCC_PB1_IDX, 27),
  RCCCan2Id = CREATE_RCC_PERIPHERAL_ID(RCC_PB1_IDX, 26),
  RCCCan1Id = CREATE_RCC_PERIPHERAL_ID(RCC_PB1_IDX, 25),
  RCCUsbDId = CREATE_RCC_PERIPHERAL_ID(RCC_PB1_IDX, 23),
  RCCI2c2Id = CREATE_RCC_PERIPHERAL_ID(RCC_PB1_IDX, 22),

  RCC_USART4_ID = CREATE_RCC_PERIPHERAL_ID(RCC_PB1_IDX, 19),
  RCC_USART3_ID = CREATE_RCC_PERIPHERAL_ID(RCC_PB1_IDX, 18),
  RCC_USART2_ID = CREATE_RCC_PERIPHERAL_ID(RCC_PB1_IDX, 17),

  RCC_SPI2_ID = CREATE_RCC_PERIPHERAL_ID(RCC_PB1_IDX, 14),

} RCCPeripheralId;

/**
 * @brief
 *
 */
struct RCCCfgValues {
  uint32_t hse_freq;     // HSE osc freq, or zero if using HSI
  uint32_t sysclk_freq;  // Wanted sysclk freq
  uint32_t hclk_freq;
  uint32_t usbclk_freq;
  uint32_t pclk1_freq;
  uint32_t pclk2_freq;
  uint32_t adcclk_freq;
  uint32_t cfgr0;
};

/**
 * @brief Set RCC block to a safe init values
 *
 */
void rcc_init(void);

/**
 * @brief Cfg clock tree based on inputs
 *
 * @param hse_freq Frequency of the High Speed Exernal OSC input (0 means use
 * HSI)
 * @param sysclk_freq Wanted system clock frequency
 */
void rcc_cfg_clock_tree(uint32_t hse_freq, uint32_t sysclk_freq);

/**
 * @brief Set clock for selected peripherial on/off
 *
 * @param id Which peripheral
 * @param on on or off
 */
void rcc_set_peripheral_clk(RCCPeripheralId id, uint32_t on);

/**
 * @brief Get the clk values object
 *
 * @return const RCCCfgValues*
 */
const struct RCCCfgValues* get_clk_values(void);