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

static const char *message =
    "Parity error flag. In the receiving mode, if a parity error occurs, this bit can be set by hardware.This bit can be reset by "
    "reading the bit and then reading the data register\n";

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

  usart_enable(USART1_ID, 1);
}

void main(void) {
  setup_led();
  setup_usart1();

  uint32_t cursor = 0;

  while (1) {
    gpio_pin_set_fast(&ledCache, 1);
    core_delay_ms(1000);
    gpio_pin_set_fast(&ledCache, 0);
    core_delay_ms(1000);

    cursor = 0;
    while (true) {
      uint8_t ch = message[cursor];
      if (ch == '\0') {
        break;
      }
      usart_send_byte(USART1_ID, ch, true);
      cursor++;
    }
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