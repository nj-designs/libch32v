/**
 * @file demo.c
 * @author Neil Johnson (nj.designs@protonmail.com)
 * @brief
 * @version 0.1
 * @date 2024-03-23
 *
 * @copyright Copyright (c) 2024
 *
 */
#include <stdbool.h>
#include <stdlib.h>

#include "core.h"
#include "dma.h"
#include "gpio.h"
#include "rcc.h"
#include "usart.h"

static struct GPIOPinSetCache ledCache;

static const struct UsartCfgValues usart_cfg_values = {
    .baud_rate = 9600,
    .word_len = USART_WORD_LEN_8_BITS,
    .parity = USART_PARITY_NONE,
    .stop_bits = USART_STOP_BITS_1_0,
    .mode = USART_DATA_MODE_RX_AND_TX,
    .dma = true,
};

static const char message1[] = "0 1 2 3 4 5 6 7 8 9\n\n";
static const char message2[] = "18.10.2 USART Data Register (USARTx_DATAR) (x=1/2/3/4/5/6/7/8)\n\n";
static const char message3[] =
    "Below is a useful subset of gdb commands, listed roughly in the order they might be needed.\n\n";
static uint32_t msg_idx;
static struct DMAXferRequest dma_reqs[3];

static void dma_cb(struct DMAXferRequest *req) {
  (void)req;

  uint32_t idx = msg_idx++;
  if (msg_idx == 3) {
    msg_idx = 0;
  }
  dma_queue_xfer_request(&dma_reqs[idx]);
}

static void setup_led(void) {
  // Setup LED
  rcc_set_peripheral_clk(RCC_IOPA_ID, 1);
  const enum GPIOPinId led_pin = PIN_PA10;
  gpio_pin_init(led_pin, PIN_MODE_OUTPUT_PUSH_PULL_50MHZ);
  gpio_pin_cache(led_pin, &ledCache);
}

static void setup_usart1(void) {
  rcc_set_peripheral_clk(RCC_USART1_ID, 1);
  rcc_set_peripheral_clk(RCC_AFIO_ID, 1);

  gpio_pin_init(PIN_PA9, PIN_MODE_ALTERNATE_FUNC_PUSH_PULL_50MHZ);

  usart_cfg(USART1_ID, &usart_cfg_values);

  usart_enable(USART1_ID, 1);
}

static void setup_dma(void) {
  rcc_set_peripheral_clk(RCC_DMA1_ID, 1);

  for (uint32_t i = 0; i < 3; i++) {
    struct DMAXferRequest *dma_req = &dma_reqs[i];
    dma_req->cb = dma_cb;
    dma_req->id = DMA_PERIPHERAL_ID_USART1_TX;

    switch (i) {
    case 0: {
      dma_req->memory_address = (void *)message1;
      dma_req->xfter_len = sizeof(message1);
      break;
    }
    case 1: {
      dma_req->memory_address = (void *)message2;
      dma_req->xfter_len = sizeof(message2);
      break;
    }
    case 2: {
      dma_req->memory_address = (void *)message3;
      dma_req->xfter_len = sizeof(message3);
      break;
    }
    default: {
      break;
    }
    }
  }
}

void main(void) {
  setup_led();
  setup_usart1();
  setup_dma();

  dma_cb(nullptr);

  while (1) {
    gpio_pin_set_fast(&ledCache, 1);
    core_delay_ms(1000);
    gpio_pin_set_fast(&ledCache, 0);
    core_delay_ms(1000);
  }
}

void int_handler_nmi(void) NJD_IRQ_ATTRIBUTE;
void int_handler_nmi(void) {
  while (1) {
    asm volatile("nop");
    asm volatile("nop");
  }
}

void int_handler_hard_fault(void) NJD_IRQ_ATTRIBUTE;
void int_handler_hard_fault(void) {
  while (1) {
    asm volatile("nop");
  }
}
