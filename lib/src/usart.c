/**
 * @file usart.c
 * @author Neil Johnson (nj.designs@protonmail.com)
 * @brief
 * @version 0.1
 * @date 2024-03-22
 *
 * @copyright Copyright (c) 2024
 *
 */
#include <stddef.h>

#include "rcc.h"
#include "usart.h"

#ifdef LIBCH32_HAS_USART1
struct USARTRegMap __attribute__((section(".usart1"))) usart1;
#endif

#ifdef LIBCH32_HAS_USART2
struct USARTRegMap __attribute__((section(".usart2"))) usart2;
#endif

#ifdef LIBCH32_HAS_USART3
struct USARTRegMap __attribute__((section(".usart3"))) usart3;
#endif

#ifdef LIBCH32_HAS_USART4
struct USARTRegMap __attribute__((section(".usart4"))) usart4;
#endif

#ifdef LIBCH32_HAS_USART5
struct USARTRegMap __attribute__((section(".usart5"))) usart5;
#endif

#ifdef LIBCH32_HAS_USART6
struct USARTRegMap __attribute__((section(".usart6"))) usart6;
#endif

#ifdef LIBCH32_HAS_USART7
struct USARTRegMap __attribute__((section(".usart7"))) usart7;
#endif

#ifdef LIBCH32_HAS_USART8
struct USARTRegMap __attribute__((section(".usart8"))) usart8;
#endif

static struct USARTRegMap* _usart_get_reg_map(UsartId id) {
  struct USARTRegMap* reg = nullptr;
  switch (id) {
#if defined(LIBCH32_HAS_USART1)
    case USART1_ID: {
      reg = &usart1;
      break;
    }
#endif
#if defined(LIBCH32_HAS_USART2)
    case USART2_ID: {
      reg = &usart2;
      break;
    }
#endif
#if defined(LIBCH32_HAS_USART3)
    case USART3_ID: {
      reg = &usart3;
      break;
    }
#endif
#if defined(LIBCH32_HAS_USART4)
    case USART4_ID: {
      reg = &usart4;
      break;
    }
#endif
#if defined(LIBCH32_HAS_USART5)
    case USART5_ID: {
      reg = &usart5;
      break;
    }
#endif
#if defined(LIBCH32_HAS_USART6)
    case USART6_ID: {
      reg = &usart6;
      break;
    }
#endif
#if defined(LIBCH32_HAS_USART7)
    case USART7_ID: {
      reg = &usart7;
      break;
    }
#endif
#if defined(LIBCH32_HAS_USART8)
    case USART8_ID: {
      reg = &usart8;
      break;
    }
#endif

    default: {
      break;
    }
  }
  return reg;
}

static uint32_t _usart_get_src_clk_freq(UsartId id) {
  enum RCCPeripheralId rcc_id;

  switch (id) {
#if defined(LIBCH32_HAS_USART2)
    case USART2_ID: {
      rcc_id = RCC_USART2_ID;
      break;
    }
#endif
#if defined(LIBCH32_HAS_USART3)
    case USART3_ID: {
      rcc_id = RCC_USART3_ID;
      break;
    }
#endif
#if defined(LIBCH32_HAS_USART4)
    case USART4_ID: {
      rcc_id = RCC_USART4_ID;
      break;
    }
#endif
#if defined(LIBCH32_HAS_USART5)
    case USART5_ID: {
      rcc_id = RCC_USART5_ID;
      break;
    }
#endif
#if defined(LIBCH32_HAS_USART6)
    case USART6_ID: {
      rcc_id = RCC_USART6_ID;
      break;
    }
#endif
#if defined(LIBCH32_HAS_USART7)
    case USART7_ID: {
      rcc_id = RCC_USART7_ID;
      break;
    }
#endif
#if defined(LIBCH32_HAS_USART8)
    case USART8_ID: {
      rcc_id = RCC_USART8_ID;
      break;
    }
#endif
    default: {
      rcc_id = RCC_USART1_ID;
      break;
    }
  }
  return rcc_get_clk_freq(rcc_get_clk_src(rcc_id));
}

void usart_cfg(UsartId id, const struct UsartCfgValues* cfg) {
  struct USARTRegMap* reg = _usart_get_reg_map(id);
  if (reg != NULL) {
    reg->ctlr1 = cfg->word_len | cfg->parity | cfg->mode;

    reg->ctlr2 = cfg->stop_bits;

    if (cfg->dma) {
      switch (cfg->mode) {
        case USART_DATA_MODE_RX_ONY: {
          reg->ctlr3 = RCC_CTRL3_DMAR;
          break;
        }

        case USART_DATA_MODE_TX_ONY: {
          reg->ctlr3 = RCC_CTRL3_DMAT;
          break;
        }

        default: {
          reg->ctlr3 = RCC_CTRL3_DMAT | RCC_CTRL3_DMAR;
          break;
        }
      }
    } else {
      reg->ctlr3 = 0;
    }

    const uint32_t src_clk = _usart_get_src_clk_freq(id);
    // Taken from USART_Init() in ch32v20x_usart.c
    uint32_t integerdivider    = ((25 * src_clk) / (4 * (cfg->baud_rate)));
    uint32_t tmpreg            = (integerdivider / 100) << 4;
    uint32_t fractionaldivider = integerdivider - (100 * (tmpreg >> 4));
    tmpreg |= ((((fractionaldivider * 16) + 50) / 100)) & ((uint8_t)0x0F);
    reg->brr = (tmpreg & 0xFFFF);

    reg->statr = 0;  // Clear any interrupts
  }
}

void usart_enable(UsartId id, uint32_t en) {
  struct USARTRegMap* reg = _usart_get_reg_map(id);
  if (reg != NULL) {
    if (en) {
      reg->ctlr1 |= RCC_CTRL1_UE;
    } else {
      reg->ctlr1 &= ~(RCC_CTRL1_UE);
    }
  }
}

void usart_send_byte(UsartId id, uint16_t value, const bool block) {
  struct USARTRegMap* reg = _usart_get_reg_map(id);
  if (reg != NULL) {
    while (block && (reg->statr & RCC_STATR_TXE) == 0) {
    }
    reg->datar = value;
  }
}

void usart_enable_interrupts(UsartId id, uint32_t ints, uint32_t en) {
  struct USARTRegMap* reg = _usart_get_reg_map(id);
  if (reg != NULL) {
    if (en) {
      reg->ctlr1 |= ints;
    } else {
      reg->ctlr1 &= ~ints;
    }
  }
}

void usart_tx_buffer_request_start(struct UsartTxBufferRequest* req) {
  struct USARTRegMap* reg = _usart_get_reg_map(req->usart_id);
  if (reg != NULL) {
    reg->statr = 0;  // Clear any interrupts
    usart_enable_interrupts(req->usart_id, RCC_CTRL1_TCIE, 1);
    req->_idx  = 0;
    reg->datar = *req->base;
  }
}

void usart_tx_buffer_request_handle_int(struct UsartTxBufferRequest* req) {
  struct USARTRegMap* reg = _usart_get_reg_map(req->usart_id);
  if (reg != NULL) {
    req->_idx++;
    req->len--;
    reg->statr = 0;  // Clear any interrupts
    if (req->len == 0) {
      if (req->cb) {
        req->cb(req);  // cb can call usart_tx_buffer_request_start to restart tx
      }
      if (req->len == 0) {
        usart_enable_interrupts(req->usart_id, RCC_CTRL1_TCIE, 0);
      }
    } else {
      reg->datar = req->base[req->_idx];  // Writing to datar will clear TC int
    }
  }
}
