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

#include "afio.h"
#include "can.h"
#include "core.h"
#include "gpio.h"
#include "printf.h"
#include "rcc.h"
#include "stdout.h"
#include <stdint.h>

static struct GPIOPinSetCache ledCache;

const enum GPIOPinId LED_PIN = PIN_PA3;

static uint8_t can_msg[8] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x7};

static const uint32_t can_ids[] = {
    CAN_STD_ID(0x555), CAN_STD_ID(0x317), CAN_STD_ID(0x400), CAN_EXT_ID(0x900), CAN_EXT_ID(0x1FFFFFFF),
};

struct CanCmd1 {
  uint8_t  opcode;
  uint16_t arg1;
  uint16_t arg2;
};
static_assert(sizeof(struct CanCmd1) <= 8, "Too big");

void can_rx_handler(const CanRxMsg *can_msg) {
  printf("CAN Msg - Id: 0x%X Len: %d PayLoad:", can_msg->id, can_msg->data_len);
  /* int len = can_msg->data_len <= 8 ? can_msg->data_len : 8;
  for (int idx = 0; idx < len; idx++) {
    printf(" %02X", can_msg->data_ptr[idx]);
  }*/
  printf("\n");

  /* if (can_msg->data_len >= sizeof(struct CanCmd1)) {
    struct CanCmd1 *cmd = (struct CanCmd1 *)can_msg->data_ptr;
    printf("CanCmd1 - opcode:0x%02X arg1:0x%04X arg2:0x%04X\n", cmd->opcode, cmd->arg1, cmd->arg2);
  } */
}

static void print_clocks(void) {

  static const struct {
    const char     *str;
    enum RCCClockId id;
  } clocks[] = {

      {
          "HSE: %d\n",
          RCC_CLOCK_ID_HSE,
      },
      {
          "HSI: %d\n",
          RCC_CLOCK_ID_HSI,
      },
      {
          "SYSCLK: %d\n",
          RCC_CLOCK_ID_SYSCLK,
      },
      {
          "HCLK: %d\n",
          RCC_CLOCK_ID_HCLK,
      },
      {
          "TIM1: %d\n",
          RCC_CLOCK_ID_TIM1,
      },
      {
          "TIM2: %d\n",
          RCC_CLOCK_ID_TIM2,
      },
      {
          "PCLK1: %d\n",
          RCC_CLOCK_ID_PCLK1,
      },
      {
          "PCLK2: %d\n",
          RCC_CLOCK_ID_PCLK2,
      },

  };
  const uint32_t clk_count = sizeof(clocks) / sizeof(clocks[0]);
  for (uint32_t idx = 0; idx < clk_count; idx++) {
    volatile uint32_t freq = rcc_get_clk_freq(clocks[idx].id);
    printf(clocks[idx].str, freq);
  }
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
  afio.pcfr1 = 0x6000;
  gpio_pin_init(PIN_PD0, PIN_MODE_INPUT_PULL_UP);
  gpio_pin_init(PIN_PD1, PIN_MODE_ALTERNATE_FUNC_PUSH_PULL_50MHZ);

  can_init(CAN_CTRL_ID_1, 500'000, true, true, can_rx_handler);
  const uint32_t id_cnt = sizeof(can_ids) / sizeof(can_ids[0]);
  can_filter_init_ex(CAN_CTRL_ID_1, can_ids, id_cnt);
}

static void tx_fail(uint32_t line_num) {
  volatile uint32_t fail_num = line_num;
  while (fail_num) {}
}

static const uint32_t MAX_CAN_WAIT_MS = 5;

void main(void) {

  stdout_init();

  print_clocks();

  setup_led();
  setup_can();

  struct CANTxReq can_req = {
      .ctrl_id = CAN_CTRL_ID_1, .data_ptr = can_msg, .data_len = 8, .id = CAN_STD_ID(0x317)
  };

  can_msg[0] = 0xA0;
  can_req.id = CAN_STD_ID(0x555);
  if (can_tx_req(&can_req, MAX_CAN_WAIT_MS) == false) {
    tx_fail(__LINE__);
  }

  can_msg[0] = 0xA1;
  can_req.id = CAN_STD_ID(0x500);
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

  can_msg[0] = 0x90;
  can_req.id = CAN_EXT_ID(0x900);
  if (can_tx_req(&can_req, MAX_CAN_WAIT_MS) == false) {
    tx_fail(__LINE__);
  }

  while (1) {
    printf("On\n");
    gpio_pin_set_fast(&ledCache, 1);
    core_delay_ms(1000);
    printf("Off\n");
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
