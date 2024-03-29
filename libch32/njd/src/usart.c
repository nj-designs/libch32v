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
    uint32_t tmpreg;

    // rcc.apb2prstr = (1 << 14);

    tmpreg = reg->ctlr2;
    tmpreg &= 0xCFFF;
    tmpreg |= cfg->stop_bits;
    reg->ctlr2 = tmpreg;

    tmpreg = reg->ctlr1;
    tmpreg &= 0xE9F3;
    tmpreg |= cfg->word_len | cfg->parity | cfg->mode;
    reg->ctlr1 = tmpreg;

    tmpreg = reg->ctlr3;
    tmpreg &= 0xFCFF;
    tmpreg |= 0;
    reg->ctlr3 = tmpreg;

    const struct RCCCfgValues* clk = get_clk_values();
    uint32_t pclk = id == USART1_ID ? clk->pclk2_freq : clk->pclk1_freq;
    // Taken from USART_Init() in ch32v20x_usart.c
    uint32_t integerdivider = ((25 * pclk) / (4 * (cfg->baud_rate)));
    tmpreg = (integerdivider / 100) << 4;
    uint32_t fractionaldivider = integerdivider - (100 * (tmpreg >> 4));
    tmpreg |= ((((fractionaldivider * 16) + 50) / 100)) & ((uint8_t)0x0F);
    reg->brr = (tmpreg & 0xFFFF);

    reg->ctlr1 |= RCC_CTRL1_UE;
  }
}

void usart_send_byte(UsartId id, uint16_t value) {
  USARTRegMap* reg = reg_lookup[(uint32_t)id];
  if (reg != NULL) {
    while ((reg->statr & RCC_STATR_TXE) == 0) {
    }
    reg->datar = value;
  }
}
