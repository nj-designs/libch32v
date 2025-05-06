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
#include "printf.h"
#include "rcc.h"

static struct GPIOPinSetCache ledCache;

const enum GPIOPinId LED_PIN = PIN_PA10;

static uint8_t can_msg[8] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x7};

static const uint32_t can_ids[4] = {CAN_STD_ID(0x555), CAN_STD_ID(0x317), CAN_STD_ID(0x400),
                                    CAN_STD_ID(0x900)};

void can_rx_handler(const CanRxMsg *can_msg) {
  (void)can_msg;
  // printf("CAN - Got %p\n", (void *)can_msg);
}

static void setup_led(void) {
  // Setup LED
  rcc_set_peripheral_clk(RCC_IOPA_ID, 1);
  gpio_pin_init(LED_PIN, PIN_MODE_OUTPUT_PUSH_PULL_50MHZ);
  gpio_pin_cache(LED_PIN, &ledCache);
}

static void setup_can(void) {

  rcc_set_peripheral_clk(RCC_AFIO_ID, 1);
  rcc_set_peripheral_clk(RCC_IOPD_ID, 1);
  gpio_pin_init(PIN_PD0, PIN_MODE_INPUT_PULL_UP);
  gpio_pin_init(PIN_PD1, PIN_MODE_ALTERNATE_FUNC_PUSH_PULL_50MHZ);

  can_init(CAN1, 500'000, true, true, can_rx_handler);
  // can_filter_init(CAN1);
  can_filter_init_ex(CAN1, can_ids, 4);
}

static void tx_fail(uint32_t line_num) {
  volatile uint32_t fail_num = line_num;
  while (fail_num) {
  }
}

static const uint32_t MAX_CAN_WAIT_MS = 5;

void main(void) {
  setup_led();
  setup_can();

  struct CANTxReq can_req = {.reg_ptr = CAN1, .data_ptr = can_msg, .data_len = 8, .id = CAN_STD_ID(0x317)};

  can_msg[0] = 0xA0;
  can_req.id = CAN_STD_ID(0x555);
  if (can_tx_req(&can_req, MAX_CAN_WAIT_MS) == false) {
    tx_fail(__LINE__);
  }

  can_msg[0] = 0xA1;
  can_req.id = CAN_STD_ID(0x900);
  if (can_tx_req(&can_req, MAX_CAN_WAIT_MS) == false) {
    tx_fail(__LINE__);
  }

  can_msg[0] = 0xA2;
  can_req.id = CAN_STD_ID(0x317);
  if (can_tx_req(&can_req, MAX_CAN_WAIT_MS) == false) {
    tx_fail(__LINE__);
  }

  can_msg[0] = 0xA3;
  can_req.id = CAN_STD_ID(0x400);
  if (can_tx_req(&can_req, MAX_CAN_WAIT_MS) == false) {
    tx_fail(__LINE__);
  }

  core_delay_ms(2000);

  can_deinit(CAN1);

  while (1) {
    // printf("On\n");
    gpio_pin_set_fast(&ledCache, 1);
    core_delay_ms(1000);
    // printf("Off\n");
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
    asm volatile("csrr a1, mcause");
    asm volatile("csrr a2, mtval");
    asm volatile("nop");
  }
}
