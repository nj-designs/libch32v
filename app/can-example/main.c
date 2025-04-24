/**
 * @file main.c
 * @author Neil Johnson (nj.designs@protonmail.com)
 * @brief CAN bus example
 * @version 0.1
 * @date 19/4/2025
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "can.h"
#include "core.h"
#include "gpio.h"
#include "rcc.h"

static struct GPIOPinSetCache ledCache;

const enum GPIOPinId LED_PIN = PIN_PA9;

static const uint8_t can_msg[8] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x7};

static void setup_led(void) {
  // Setup LED
  rcc_set_peripheral_clk(RCC_IOPA_ID, 1);
  gpio_pin_init(LED_PIN, PIN_MODE_OUTPUT_PUSH_PULL_50MHZ);
  gpio_pin_cache(LED_PIN, &ledCache);
}

void setup_can(void) {

  rcc_set_peripheral_clk(RCC_AFIO_ID, 1);
  rcc_set_peripheral_clk(RCC_IOPD_ID, 1);
  gpio_pin_init(PIN_PD0, PIN_MODE_INPUT_PULL_UP);
  gpio_pin_init(PIN_PD1, PIN_MODE_ALTERNATE_FUNC_PUSH_PULL_50MHZ);

  can_init(CAN1, 500'000, true, true);
  can_filter_init(CAN1);
}

void main(void) {
  setup_led();
  setup_can();

  struct CANTxReq can_req = {.reg_ptr = CAN1, .data_ptr = can_msg, .data_len = 8, .id = CAN_STD_ID(0x317)};
  can_tx_req(&can_req);
  volatile enum CanTxStatus tx_status = CAN_TX_RUNNING;
  do {
    tx_status = can_check_tx_complete(&can_req);
  } while (tx_status == CAN_TX_RUNNING);
  can_deinit(CAN1);

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
