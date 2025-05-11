/**
 * @file stdout-buffer.c
 * @author Neil Johnson (nj.designs@protonmail.com)
 * @brief
 * @version 0.1
 * @date 2024-04-02
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "stdout.h"

#include "core.h"
#include "dma.h"
#include "gpio.h"
#include "rcc.h"
#include "usart.h"

#if !defined(APP_STDOUT_BUFFER_SIZE)
#define APP_STDOUT_BUFFER_SIZE (256)
#endif

static void dma_cb(struct DMAXferRequest *req);

#if (LIBCH32_DEVICE_ID == WCH_CH32V203G6U6) || (LIBCH32_DEVICE_ID == WCH_CH32V307VCT6)
static const enum GPIOPinId USART1_TX_PIN = PIN_PA9;
#elif LIBCH32_DEVICE_ID == WCH_CH32V003F4
static const enum GPIOPinId USART1_TX_PIN = PIN_PD5;
#else
#error "unsupported device"
#endif

static const struct UsartCfgValues usart_cfg_values = {
    .baud_rate = 115200,
    .word_len = USART_WORD_LEN_8_BITS,
    .parity = USART_PARITY_NONE,
    .stop_bits = USART_STOP_BITS_1_0,
    .mode = USART_DATA_MODE_TX_ONY,
    .dma = true,
};

static uint8_t               op_buffer[APP_STDOUT_BUFFER_SIZE];
static uint64_t              wr_idx;
static uint64_t              rd_idx;
static struct DMAXferRequest dma_req = {
    .cb = dma_cb,
    .id = DMA_PERIPHERAL_ID_USART1_TX,
};
static volatile bool dma_in_progress;

static void dma_cb(struct DMAXferRequest *req) {
  if (dma_in_progress) {
    rd_idx += req->xfter_len;
    dma_in_progress = false;
  } else {
    while (1) {}
  }
}

void stdout_init(void) {
  rcc_set_peripheral_clk(RCC_IOPA_ID, 1);
  rcc_set_peripheral_clk(RCC_USART1_ID, 1);
  rcc_set_peripheral_clk(RCC_AFIO_ID, 1);
  rcc_set_peripheral_clk(RCC_DMA1_ID, 1);

  gpio_pin_init(USART1_TX_PIN, PIN_MODE_ALTERNATE_FUNC_PUSH_PULL_50MHZ);

  usart_cfg(USART1_ID, &usart_cfg_values);

  core_pfic_set_int_priority(PFIC_DMA1_CH4_INT_NUM, PFIC_INT_PRIORITY_HIGHEST);

  usart_enable(USART1_ID, 1);
}

static void drain_buffer(uint32_t count) {
  while (count) {
    while (dma_in_progress) {}
    uint32_t head_room = APP_STDOUT_BUFFER_SIZE - (rd_idx & (APP_STDOUT_BUFFER_SIZE - 1));
    uint32_t xfter_len = count > head_room ? count - head_room : count;
    count -= xfter_len;
    dma_req.xfter_len = xfter_len;
    dma_req.memory_address = &op_buffer[rd_idx & (APP_STDOUT_BUFFER_SIZE - 1)];
    dma_in_progress = true;
    dma_queue_xfer_request(&dma_req);
  }
}

void _putchar(char ch) {
  bool flush = ch == '\n' ? true : false;
  op_buffer[wr_idx & (APP_STDOUT_BUFFER_SIZE - 1)] = (uint8_t)ch;
  wr_idx++;
  if (dma_in_progress) {
    if (flush) {
      while (dma_in_progress) {}
      uint32_t count = (uint32_t)(wr_idx - rd_idx);
      drain_buffer(count);
    }
  } else {
    uint32_t count = (uint32_t)(wr_idx - rd_idx);
    if (flush || count >= APP_STDOUT_BUFFER_SIZE / 4) {
      drain_buffer(count);
    }
  }
}
