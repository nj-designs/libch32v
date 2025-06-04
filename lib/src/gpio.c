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

struct GPIORegMap {
  volatile uint32_t cfglr;
  volatile uint32_t cfghr;
  volatile uint32_t indr;
  volatile uint32_t outdr;
  volatile uint32_t bshr;
  volatile uint32_t bcr;
  volatile uint32_t lckr;
};

#ifdef LIBCH32_HAS_GPIOA
static struct GPIORegMap __attribute__((section(".gpio_a"))) gpio_a;
#endif

#ifdef LIBCH32_HAS_GPIOB
static struct GPIORegMap __attribute__((section(".gpio_b"))) gpio_b;
#endif

#ifdef LIBCH32_HAS_GPIOC
static struct GPIORegMap __attribute__((section(".gpio_c"))) gpio_c;
#endif

#ifdef LIBCH32_HAS_GPIOD
static struct GPIORegMap __attribute__((section(".gpio_d"))) gpio_d;
#endif

#ifdef LIBCH32_HAS_GPIOE
static struct GPIORegMap __attribute__((section(".gpio_e"))) gpio_e;
#endif

static struct GPIORegMap* _gpio_get_reg_map(enum GPIOBlockId id) {
  struct GPIORegMap* reg = nullptr;
  switch (id) {
#ifdef LIBCH32_HAS_GPIOA
    case GPIO_A_IDX: {
      reg = &gpio_a;
      break;
    }
#endif

#ifdef LIBCH32_HAS_GPIOB
    case GPIO_B_IDX: {
      reg = &gpio_b;
      break;
    }
#endif

#ifdef LIBCH32_HAS_GPIOC
    case GPIO_C_IDX: {
      reg = &gpio_c;
      break;
    }
#endif

#ifdef LIBCH32_HAS_GPIOD
    case GPIO_D_IDX: {
      reg = &gpio_d;
      break;
    }
#endif

#ifdef LIBCH32_HAS_GPIOE
    case GPIO_E_IDX: {
      reg = &gpio_e;
      break;
    }
#endif

    default: {
      break;
    }
  }
  return reg;
}
void gpio_pin_init(enum GPIOPinId pin_id, GPIOPinMode mode) {
  struct GPIORegMap* port = _gpio_get_reg_map(pin_id >> 16);
  if (port != nullptr) {
    uint32_t pin_num = pin_id & 0xFFFF;
    uint32_t cfg_value;

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
        } else {
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
}

void gpio_pin_set(enum GPIOPinId pin_id, uint8_t val) {
  struct GPIORegMap* port = _gpio_get_reg_map(pin_id >> 16);
  if (port != nullptr) {
    uint16_t pin_bit = 1 << ((uint16_t)pin_id & 0xFFFF);
    if (val) {
      port->bshr = pin_bit;
    } else {
      port->bcr = pin_bit;
    }
  }
}

void gpio_pin_cache(enum GPIOPinId pin_id, struct GPIOPinSetCache* cache) {
  struct GPIORegMap* port = _gpio_get_reg_map(pin_id >> 16);
  // TODO(njohn) : Handle nullptr case better
  if (port != nullptr) {
    cache->bshr        = &port->bshr;
    cache->bcr         = &port->bcr;
    cache->pin_bit_map = 1 << ((uint16_t)pin_id & 0xFFFF);
  }
}
