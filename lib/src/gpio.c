/**
 * @file gpio.c
 * @author Neil Johnson (nj.designs@protonmail.com)
 * @brief
 * @version 0.1
 * @date 2024-03-20
 *
 * @copyright Copyright (c) 2024
 *
 */
#include <stddef.h>

#include "gpio.h"

#ifdef LIBCH32_HAS_GPIOA
struct GPIORegMap __attribute__((section(".gpio_a"))) gpio_a;
#endif

#ifdef LIBCH32_HAS_GPIOB
struct GPIORegMap __attribute__((section(".gpio_b"))) gpio_b;
#endif

#ifdef LIBCH32_HAS_GPIOC
struct GPIORegMap __attribute__((section(".gpio_c"))) gpio_c;
#endif

#ifdef LIBCH32_HAS_GPIOD
struct GPIORegMap __attribute__((section(".gpio_d"))) gpio_d;
#endif

#ifdef LIBCH32_HAS_GPIOE
struct GPIORegMap __attribute__((section(".gpio_e"))) gpio_e;
#endif

#if LIBCH32_DEVICE_ID == WCH_CH32V307VCT6
static struct GPIORegMap *const port_lookup[] = {
    &gpio_a, // GPIO_A_IDX = 0
    &gpio_b, // GPIO_B_IDX = 1
    &gpio_c, // GPIO_C_IDX = 2
    &gpio_d, // GPIO_D_IDX = 3
    &gpio_e  // GPIO_E_IDX = 4
};
#elif LIBCH32_DEVICE_ID == WCH_CH32V203G6U6
static struct GPIORegMap *const port_lookup[] = {
    &gpio_a, // GPIO_A_IDX = 0
    &gpio_b, // GPIO_B_IDX = 1
    NULL,    // GPIO_C_IDX = 2 (Not present)
    &gpio_d, // GPIO_D_IDX = 3
    NULL     // GPIO_E_IDX = 4  (Not present)
};
#elif LIBCH32_DEVICE_ID == WCH_CH32V003F4
static struct GPIORegMap *const port_lookup[] = {
    &gpio_a, // GPIO_A_IDX = 0
    NULL,    // GPIO_B_IDX = 1 (Not present)
    &gpio_c, // GPIO_C_IDX = 2
    &gpio_d, // GPIO_D_IDX = 3
    NULL     // GPIO_E_IDX = 4  (Not present)
};
#elif LIBCH32_DEVICE_ID == WCH_CH32V203C8T6
static struct GPIORegMap *const port_lookup[] = {
    &gpio_a, // GPIO_A_IDX = 0
    &gpio_b, // GPIO_B_IDX = 1
    nullptr, // GPIO_C_IDX = 2 (Not present)
    nullptr, // GPIO_D_IDX = 3 (Not present)
    nullptr  // GPIO_E_IDX = 4  (Not present)
};
#else
#erorr "unsupported device"
#endif

void gpio_pin_init(enum GPIOPinId pin_id, GPIOPinMode mode) {
  struct GPIORegMap *port = port_lookup[(pin_id >> 16)];
  uint32_t           pin_num = pin_id & 0xFFFF;
  uint32_t           cfg_value;

  if (pin_num > 7) {
    cfg_value = port->cfghr;
    cfg_value &= (uint32_t)~(0b1111 << (pin_num - 8) * 4);
    cfg_value |= ((uint32_t)mode & 0b1111) << (pin_num - 8) * 4;
    port->cfghr = cfg_value;
  } else {
    cfg_value = port->cfglr;
    cfg_value &= (uint32_t)~(0b1111 << pin_num * 4);
    cfg_value |= ((uint32_t)mode & 0b1111) << pin_num * 4;
    port->cfglr = cfg_value;
  }

  switch (mode) {
    case PIN_MODE_INPUT_PULL_UP:
    case PIN_MODE_INPUT_PULL_DOWN: {
      uint16_t dr = port->outdr;
    if ((uint32_t)mode & 0b1'0000) {
      dr |= (uint16_t)(1 << pin_num);
    }
      else {
        dr &= (uint16_t)~(1 << pin_num);
      }
      port->outdr = dr;
      break;
  }
  default: {
    break;
  }
}
}

void gpio_pin_set(enum GPIOPinId pin_id, uint8_t val) {
  struct GPIORegMap *port = port_lookup[(pin_id >> 16)];
  uint16_t           pin_bit = 1 << ((uint16_t)pin_id & 0xFFFF);
  if (val) {
    port->bshr = pin_bit;
  } else {
    port->bcr = pin_bit;
  }
}

void gpio_pin_cache(enum GPIOPinId pin_id, struct GPIOPinSetCache *cache) {
  struct GPIORegMap *port = port_lookup[(pin_id >> 16)];
  cache->bshr = &port->bshr;
  cache->bcr = &port->bcr;
  cache->pin_bit_map = 1 << ((uint16_t)pin_id & 0xFFFF);
}
