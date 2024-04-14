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
#include "i2c.h"

#include "device_config.h"
#include "app_config.h"

#include "printf.h"

static GPIOPinSetCache ledCache;

static const GPIOPinId triggerPin = PIN_PB5;

static const struct I2CCfgValues i2c1_cfg = {
    .bus_speed = 50'000,
    .duty_cycle = I2C_DUTY_CYCLE_16_9,
    .ack = true,
    .ack_addr = I2C_ACK_ADDR_7_BIT,
};

static void setup_led(void) {
  // Setup LED
  rcc_set_peripheral_clk(RCC_IOPA_ID, 1);
  rcc_set_peripheral_clk(RCC_IOPB_ID, 1);
  const GPIOPinId led_pin = PIN_PA3;
  gpio_pin_init(led_pin, PIN_MODE_OUTPUT_PUSH_PULL_50MHZ);
  gpio_pin_cache(led_pin, &ledCache);

  gpio_pin_init(triggerPin, PIN_MODE_OUTPUT_PUSH_PULL_50MHZ);
  gpio_pin_set(triggerPin, 1);
}

static void setup_i2c(void) {
  rcc_set_peripheral_clk(RCC_IOPB_ID, 1);
  rcc_set_peripheral_clk(RCC_AFIO_ID, 1);
  rcc_set_peripheral_clk(RCC_I2C1_ID, 1);

  gpio_pin_init(PIN_PB6, PIN_MODE_ALTERNATE_FUNC_OPEN_DRAIN_10MHZ);  // SCL
  gpio_pin_init(PIN_PB7, PIN_MODE_ALTERNATE_FUNC_OPEN_DRAIN_10MHZ);  // SDA

  i2c_cfg(I2C_ID_1, &i2c1_cfg);

  i2c_enable(I2C_ID_1, 1);

  // TODO(njohn) : Fix me!!
  // i2c1.ckcfgr = I2C_CKCFGR_FnS | I2C_CKCFGR_DUTY | 150;
  // i2c1.ctlr2 = 60;
#if 0
  printf("I2C1 Regs\n");
  printf("ctlr1: 0x%04x\n", i2c1.ctlr1);
  printf("ctlr2: 0x%04x\n", i2c1.ctlr2);
  printf("oaddr1: 0x%04x\n", i2c1.oaddr1);
  printf("oaddr2: 0x%04x\n", i2c1.oaddr2);
  printf("star1: 0x%04x\n", i2c1.star1);
  printf("star2: 0x%04x\n", i2c1.star2);
  printf("ckcfgr: 0x%04x\n", i2c1.ckcfgr);
  printf("rtr: 0x%04x\n", i2c1.rtr);
#endif
}

static constexpr uint16_t first_i2c_addr = 0x00;
static constexpr uint16_t last_i2c_addr = 0x7F;

static int8_t device_present[128] = {
    0,
};

void main(void) {
  setup_led();
  setup_i2c();

  core_delay_ms(2000);

  gpio_pin_set(triggerPin, 0);
  core_delay_ms(1);

  for (uint16_t addr = first_i2c_addr; addr <= last_i2c_addr; addr++) {
    int32_t res = i2c_connect(I2C_ID_1, addr);
    int8_t present = 0;
    if (res < 0) {
      // printf("No device at: 0x%x err: %d\n", addr, res);
      present = -1;
    } else {
      printf("Device at: 0x%x\n", addr);
      present = 1;
      i2c_disconnect(I2C_ID_1);
    }
    if (addr < 128) {
      device_present[addr] = present;
    }
    core_delay_ms(1);
  }

  i2c_enable(I2C_ID_1, 0);

  printf("I2C devices. Present: <Hex addr> Not Present: -- Unkown: ??\n");

  uint32_t idx = 0;
  while (idx < 128) {
    printf("0x%02x\t", idx);
    for (uint32_t col = 0; col < 16 && idx < 128; col++, idx++) {
      switch (device_present[idx]) {
        case -1: {
          printf("-- ");
          break;
        }
        case 1: {
          printf("%02x ", idx);
          break;
        }
        default: {
          printf("?? ");
          break;
        }
      }
    }
    printf("\n");
  }

  while (1) {
    printf("Loop\n");
    gpio_pin_set_fast(&ledCache, 1);
    core_delay_ms(500);
    gpio_pin_set_fast(&ledCache, 0);
    core_delay_ms(500);
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
