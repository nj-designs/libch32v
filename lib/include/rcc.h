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
enum RCCPeripheralId {
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
  RCC_DMA1_ID = CREATE_RCC_PERIPHERAL_ID(RCC_AHB_IDX, 0),

  // PB1
  RCCDacId = CREATE_RCC_PERIPHERAL_ID(RCC_PB1_IDX, 29),
  RCCPwrIf5Id = CREATE_RCC_PERIPHERAL_ID(RCC_PB1_IDX, 28),
  RCCBkpId = CREATE_RCC_PERIPHERAL_ID(RCC_PB1_IDX, 27),
  RCCCan2Id = CREATE_RCC_PERIPHERAL_ID(RCC_PB1_IDX, 26),
  RCCCan1Id = CREATE_RCC_PERIPHERAL_ID(RCC_PB1_IDX, 25),
  RCCUsbDId = CREATE_RCC_PERIPHERAL_ID(RCC_PB1_IDX, 23),
  RCC_I2C2_ID = CREATE_RCC_PERIPHERAL_ID(RCC_PB1_IDX, 22),
  RCC_I2C1_ID = CREATE_RCC_PERIPHERAL_ID(RCC_PB1_IDX, 21),

  RCC_USART4_ID = CREATE_RCC_PERIPHERAL_ID(RCC_PB1_IDX, 19),
  RCC_USART3_ID = CREATE_RCC_PERIPHERAL_ID(RCC_PB1_IDX, 18),
  RCC_USART2_ID = CREATE_RCC_PERIPHERAL_ID(RCC_PB1_IDX, 17),

  RCC_SPI2_ID = CREATE_RCC_PERIPHERAL_ID(RCC_PB1_IDX, 14),

  RCC_TIM4EN_ID = CREATE_RCC_PERIPHERAL_ID(RCC_PB1_IDX, 2),
  RCC_TIM3EN_ID = CREATE_RCC_PERIPHERAL_ID(RCC_PB1_IDX, 1),
  RCC_TIM2EN_ID = CREATE_RCC_PERIPHERAL_ID(RCC_PB1_IDX, 0),

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
  RCC_IOPD_ID = CREATE_RCC_PERIPHERAL_ID(RCC_PB2_IDX, 5),
  RCC_IOPC_ID = CREATE_RCC_PERIPHERAL_ID(RCC_PB2_IDX, 5),
  RCC_IOPB_ID = CREATE_RCC_PERIPHERAL_ID(RCC_PB2_IDX, 3),
  RCC_IOPA_ID = CREATE_RCC_PERIPHERAL_ID(RCC_PB2_IDX, 2),
  RCC_AFIO_ID = CREATE_RCC_PERIPHERAL_ID(RCC_PB2_IDX, 0),

};

/**
 * @brief Enumerate possible clocks
 *
 */
enum RCCClockId {
  RCC_CLOCK_ID_HSE,
  RCC_CLOCK_ID_HSI,
  RCC_CLOCK_ID_SYSCLK,
  RCC_CLOCK_ID_HCLK,
  RCC_CLOCK_ID_TIM1,
  RCC_CLOCK_ID_TIM2,
  RCC_CLOCK_ID_PCLK1,
  RCC_CLOCK_ID_PCLK2,
};

/**
 * @brief Set RCC block to a safe init values
 *
 */
void rcc_init(void);

/**
 * @brief Set clock for selected peripherial on/off
 *
 * @param id Which peripheral
 * @param on on or off
 */
void rcc_set_peripheral_clk(enum RCCPeripheralId id, uint32_t on);

/**
 * @brief
 *
 * @param id
 */
void rcc_reset_peripherial(enum RCCPeripheralId id);

/**
 * @brief Return frequency of specified clock source
 *
 * @param clock-id
 *
 */
uint32_t rcc_get_clk_freq(enum RCCClockId clock_id);
