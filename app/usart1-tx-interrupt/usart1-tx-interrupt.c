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
#include <stdlib.h>
#include <stdbool.h>

#include "rcc.h"
#include "core.h"
#include "gpio.h"
#include "usart.h"

#include "device_config.h"

static GPIOPinSetCache ledCache;

static const struct UsartCfgValues usart_cfg_values = {
    .baud_rate = 115200,
    .word_len = USART_WORD_LEN_8_BITS,
    .parity = USART_PARITY_NONE,
    .stop_bits = USART_STOP_BITS_1_0,
    .mode = USART_DATA_MODE_RX_AND_TX,
};

static const char message1[] = "0 1 2 3 4 5 6 7 8 9\n";
static const char message2[] = "18.10.2 USART Data Register (USARTx_DATAR) (x=1/2/3/4/5/6/7/8)\n";
static const char message3[] = "Below is a useful subset of gdb commands, listed roughly in the order they might be needed.\n";
static uint32_t msg_idx;

static struct UsartTxBufferRequest tx_req;

static void tx_bc(struct UsartTxBufferRequest* req) {
  switch (msg_idx) {
    case 0: {
      req->base = (const uint8_t*)message1;
      req->len = sizeof(message1);
      msg_idx++;
      break;
    }
    case 1: {
      req->base = (const uint8_t*)message2;
      req->len = sizeof(message2);
      msg_idx++;
      break;
    }
    case 2: {
      req->base = (const uint8_t*)message3;
      req->len = sizeof(message3);
      msg_idx = 0;
      break;
    }
  }
  usart_tx_buffer_request_start(req);
}

static void setup_led(void) {
  // Setup LED
  rcc_set_peripheral_clk(RCC_IOPA_ID, 1);
  const GPIOPinId led_pin = PIN_PA3;
  gpio_pin_init(led_pin, PIN_MODE_OUTPUT_PUSH_PULL_50MHZ);
  gpio_pin_cache(led_pin, &ledCache);
}

static void setup_usart1(void) {
  rcc_set_peripheral_clk(RCC_USART1_ID, 1);
  rcc_set_peripheral_clk(RCC_AFIO_ID, 1);

  gpio_pin_init(PIN_PA9, PIN_MODE_ALTERNATE_FUNC_PUSH_PULL_50MHZ);

  usart_cfg(USART1_ID, &usart_cfg_values);

  core_enable_pfic_irq(PFIC_USART1_INT_NUM);

  usart_enable(USART1_ID, 1);
}

void main(void) {
  rcc_cfg_clock_tree(LIBCH32_HSE_FREQ, LIBCH32_SYSCLK_FREQ);

  setup_led();
  setup_usart1();

  tx_req.base = (const uint8_t*)message1;
  tx_req.len = sizeof(message1) - 1;  // Don't send '\0'
  tx_req.usart_id = USART1_ID;
  tx_req.cb = tx_bc;
  usart_tx_buffer_request_start(&tx_req);

  while (1) {
    gpio_pin_set_fast(&ledCache, 1);
    core_delay_ms(1000);
    gpio_pin_set_fast(&ledCache, 0);
    core_delay_ms(1000);
  }
}

void int_handler_usart1(void) NJD_IRQ_ATTRIBUTE;
void int_handler_usart1(void) { usart_tx_buffer_request_handle_int(&tx_req); }

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