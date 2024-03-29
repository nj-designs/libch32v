/**
 * @file usart.h
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

#include "device_config.h"

typedef struct {
  volatile uint32_t statr;
  volatile uint32_t datar;
  volatile uint32_t brr;
  volatile uint32_t ctlr1;
  volatile uint32_t ctlr2;
  volatile uint32_t ctlr3;
  volatile uint32_t gpr;
} USARTRegMap;

// typedef struct {
//   volatile uint16_t statr;
//   volatile uint16_t rsvd0;
//   volatile uint16_t datar;
//   volatile uint16_t rsvd1;
//   volatile uint16_t brr;
//   volatile uint16_t rsvd2;
//   volatile uint16_t ctlr1;
//   volatile uint16_t rsvd3;
//   volatile uint16_t ctlr2;
//   volatile uint16_t rsvd4;
//   volatile uint16_t ctlr3;
//   volatile uint16_t rsvd5;
//   volatile uint16_t gpr;
//   volatile uint16_t rsvd6;
// } USARTRegMap;

// STATR
//------
// TXE[7]
static const uint32_t RCC_STATR_TXE = (1 << 7);
// TC[6]
static const uint32_t RCC_STATR_TC = (1 << 6);

// CTLR1
//------
// UE[13]
static const uint32_t RCC_CTRL1_UE = (1 << 13);
// M[12]
static const uint32_t RCC_CTRL1_M = (1 << 12);
// PCE[10]
static const uint32_t RCC_CTRL1_PCE = (1 << 10);
// PS[9]
static const uint32_t RCC_CTRL1_PS = (1 << 9);
// TE[3]
static const uint32_t RCC_CTRL1_TE = (1 << 3);
// RE[2]
static const uint32_t RCC_CTRL1_RE = (1 << 2);

// CTLR2
//------
// STOP[13:12]
static const uint32_t RCC_CTRL2_STOP_MASK = (0b00 << 12);
static const uint32_t RCC_CTRL2_STOP_1_BIT = (0b00 << 12);
static const uint32_t RCC_CTRL2_STOP_05_BIT = (0b01 << 12);
static const uint32_t RCC_CTRL2_STOP_2_BITS = (0b10 << 12);
static const uint32_t RCC_CTRL2_STOP_15_BITS = (0b11 << 12);

#ifdef LIBCH32_HAS_USART1
extern USARTRegMap usart1;
#endif

#ifdef LIBCH32_HAS_USART2
extern USARTRegMap usart2;
#endif

/**
 * @brief Enumerate all possible usart instances.
 *
 * Note: Actual availability is based on specific chip
 *
 */
typedef enum {
  USART1_ID,
  USART2_ID,
  USART3_ID,
  USART4_ID,
  USART5_ID,
  USART6_ID,
  USART7_ID,
  USART8_ID,
} UsartId;

typedef enum {
  USART_PARITY_NONE = 0,
  // Fix following once vscode supports C23 parsing.... :-(
  // USART_PARITY_EVEN = (RCC_CTRL1_PCE),
  USART_PARITY_EVEN = (1 << 10),
  // USART_PARITY_ODD = (RCC_CTRL1_PCE | RCC_CTRL1_PS),
  USART_PARITY_ODD = ((1 << 10) | (1 << 9)),
} UsartParityMode;

typedef enum {
  USART_WORD_LEN_8_BITS = 0,
  USART_WORD_LEN_9_BITS = (1 << 12),
} UsartWordLen;

typedef enum {
  USART_DATA_MODE_RX_ONY = (1 << 2),
  USART_DATA_MODE_TX_ONY = (1 << 3),
  USART_DATA_MODE_RX_AND_TX = (1 << 2) | (1 << 3),
} UsartMode;

typedef enum {
  USART_STOP_BITS_1_0 = 0,
  USART_STOP_BITS_0_5 = (0b01 << 12),
  USART_STOP_BITS_2_0 = (0b10 << 12),
  USART_STOP_BITS_1_5 = (0b11 << 12),
} UsartStopBits;

struct UsartCfgValues {
  uint32_t baud_rate;
  UsartWordLen word_len;
  UsartParityMode parity;
  UsartStopBits stop_bits;
  UsartMode mode;
};

/**
 * @brief Configure the specified usart
 *
 * @param id Which uart
 * @param cfg Config values
 */
void usart_cfg(UsartId id, const struct UsartCfgValues* cfg);

/**
 * @brief Send supplied byte via specified usart
 *
 * Note: Blocks until byte can be sent
 *
 * @param id
 * @param value
 */
void usart_send_byte(UsartId id, uint16_t value);