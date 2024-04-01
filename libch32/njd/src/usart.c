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

#include "usart.h"
#include "rcc.h"

#include "device_config.h"

#ifdef LIBCH32_HAS_USART1
USARTRegMap __attribute__((section(".usart1"))) usart1;
#endif

#ifdef LIBCH32_HAS_USART2
USARTRegMap __attribute__((section(".usart2"))) usart2;
#endif

#if LIBCH32_DEVICE_ID == WCH_CH32V203G6U6
static USARTRegMap* const reg_lookup[] = {
    &usart1,  // USART1_ID
    &usart2,  // USART2_ID
    NULL,     // USART3_ID
    NULL,     // USART4_ID
    NULL,     // USART5_ID
    NULL,     // USART6_ID
    NULL,     // USART7_ID
    NULL,     // USART8_ID
};
#else
#erorr "unsupported device"
#endif

void usart_cfg(UsartId id, const struct UsartCfgValues* cfg) {
  USARTRegMap* reg = reg_lookup[(uint32_t)id];
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

    const struct RCCCfgValues* clk = get_clk_values();
    uint32_t pclk = id == USART1_ID ? clk->pclk2_freq : clk->pclk1_freq;
    // Taken from USART_Init() in ch32v20x_usart.c
    uint32_t integerdivider = ((25 * pclk) / (4 * (cfg->baud_rate)));
    uint32_t tmpreg = (integerdivider / 100) << 4;
    uint32_t fractionaldivider = integerdivider - (100 * (tmpreg >> 4));
    tmpreg |= ((((fractionaldivider * 16) + 50) / 100)) & ((uint8_t)0x0F);
    reg->brr = (tmpreg & 0xFFFF);

    reg->statr = 0;  // Clear any interrupts
  }
}

void usart_enable(UsartId id, uint32_t en) {
  USARTRegMap* reg = reg_lookup[(uint32_t)id];
  if (reg != NULL) {
    if (en) {
      reg->ctlr1 |= RCC_CTRL1_UE;
    } else {
      reg->ctlr1 &= ~(RCC_CTRL1_UE);
    }
  }
}

void usart_send_byte(UsartId id, uint16_t value, const bool block) {
  USARTRegMap* reg = reg_lookup[(uint32_t)id];
  if (reg != NULL) {
    while (block && (reg->statr & RCC_STATR_TXE) == 0) {
    }
    reg->datar = value;
  }
}

void usart_enable_interrupts(UsartId id, uint32_t ints, uint32_t en) {
  USARTRegMap* reg = reg_lookup[(uint32_t)id];
  if (reg != NULL) {
    if (en) {
      reg->ctlr1 |= ints;
    } else {
      reg->ctlr1 &= ~ints;
    }
  }
}

void usart_tx_buffer_request_start(struct UsartTxBufferRequest* req) {
  USARTRegMap* reg = reg_lookup[(uint32_t)req->usart_id];
  if (reg != NULL) {
    reg->statr = 0;  // Clear any interrupts
    usart_enable_interrupts(req->usart_id, RCC_CTRL1_TCIE, 1);
    req->_idx = 0;
    reg->datar = *req->base;
  }
}

void usart_tx_buffer_request_handle_int(struct UsartTxBufferRequest* req) {
  USARTRegMap* reg = reg_lookup[(uint32_t)req->usart_id];
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