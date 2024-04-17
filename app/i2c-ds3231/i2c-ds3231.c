/**
 * @file i2c-ds3231.c
 * @author Neil Johnson (nj.designs@protonmail.com)
 * @brief
 * @version 0.1
 * @date 2024-04-15
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

#include "ds3231.h"

static GPIOPinSetCache ledCache;

static const GPIOPinId triggerPin = PIN_PB5;

static const struct I2CCfgValues i2c1_cfg = {
    .bus_speed = 100'000,
    .duty_cycle = I2C_DUTY_CYCLE_16_9,
    .ack = true,
    .ack_addr = I2C_ACK_ADDR_7_BIT,
};

static struct DS3231 ds3231;

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
}

void main(void) {
  int32_t res;
  // uint32_t idx;
  struct DS3231DateTime dt;

  setup_led();
  setup_i2c();

  ds3231_init(I2C_ID_1, DS3231_DEFAULT_I2C_ADDR, &ds3231);

  // Trigger pin for I2C analyser - Triggers on falling edge
  gpio_pin_set(triggerPin, 0);
  core_delay_ms(1);
  gpio_pin_set(triggerPin, 1);

  res = ds3231_refresh(&ds3231);
  if (res != 0) {
    printf("ds3231 refresh failed: %d\n", res);
    goto loop;
  }

  res = ds3231_get_date_time(&ds3231, &dt, false);
  if (res != 0) {
    printf("ds3231 get date/time failed: %d\n", res);
    goto loop;
  }

  printf("Time: %02d:%02d:%02d\n", dt.hours, dt.mins, dt.secs);

loop:
  while (1) {
    printf("Loop\n");
    gpio_pin_set_fast(&ledCache, 1);
    core_delay_ms(500);
    gpio_pin_set_fast(&ledCache, 0);
    core_delay_ms(500);

    res = ds3231_get_date_time(&ds3231, &dt, true);
    if (res != 0) {
      printf("ds3231 get date/time failed: %d\n", res);
    } else {
      printf("Time: %02d:%02d:%02d\n", dt.hours, dt.mins, dt.secs);
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
