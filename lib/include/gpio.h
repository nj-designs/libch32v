/**
 * @file gpio.h
 * @author Neil Johnson (nj.designs@protonmail.com)
 * @brief
 * @version 0.1
 * @date 2024-03-20
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once

#include <stdint.h>

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
extern struct GPIORegMap gpio_a;
#endif

#ifdef LIBCH32_HAS_GPIOB
extern struct GPIORegMap gpio_b;
#endif

#ifdef LIBCH32_HAS_GPIOC
extern struct GPIORegMap gpio_c;
#endif

#ifdef LIBCH32_HAS_GPIOD
extern struct GPIORegMap gpio_d;
#endif

#ifdef LIBCH32_HAS_GPIOE
extern struct GPIORegMap gpio_e;
#endif

#define CREATE_PIN_NUMBER(GPIO_IDX, PIN_IDX) ((GPIO_IDX) << 16 | PIN_IDX)

#define GPIO_A_IDX 0
#define GPIO_B_IDX 1
#define GPIO_C_IDX 2
#define GPIO_D_IDX 3
#define GPIO_E_IDX 4

#if LIBCH32_DEVICE_ID == WCH_CH32V203G6U6
/**
 * @brief Enumerate all valid port pins.
 *
 * Port idx & pin number are encoded in values
 */
enum GPIOPinId {
  PIN_PA0 = CREATE_PIN_NUMBER(GPIO_A_IDX, 0),
  PIN_PA1 = CREATE_PIN_NUMBER(GPIO_A_IDX, 1),
  PIN_PA2 = CREATE_PIN_NUMBER(GPIO_A_IDX, 2),
  PIN_PA3 = CREATE_PIN_NUMBER(GPIO_A_IDX, 3),
  PIN_PA4 = CREATE_PIN_NUMBER(GPIO_A_IDX, 4),
  PIN_PA5 = CREATE_PIN_NUMBER(GPIO_A_IDX, 5),
  PIN_PA6 = CREATE_PIN_NUMBER(GPIO_A_IDX, 6),
  PIN_PA7 = CREATE_PIN_NUMBER(GPIO_A_IDX, 7),
  PIN_PA9 = CREATE_PIN_NUMBER(GPIO_A_IDX, 9),
  PIN_PA10 = CREATE_PIN_NUMBER(GPIO_A_IDX, 10),
  PIN_PA11 = CREATE_PIN_NUMBER(GPIO_A_IDX, 11),
  PIN_PA12 = CREATE_PIN_NUMBER(GPIO_A_IDX, 12),
  PIN_PA13 = CREATE_PIN_NUMBER(GPIO_A_IDX, 13),
  PIN_PA14 = CREATE_PIN_NUMBER(GPIO_A_IDX, 14),
  PIN_PA15 = CREATE_PIN_NUMBER(GPIO_A_IDX, 15),
  PIN_PB0 = CREATE_PIN_NUMBER(GPIO_B_IDX, 0),
  PIN_PB1 = CREATE_PIN_NUMBER(GPIO_B_IDX, 1),
  PIN_PB3 = CREATE_PIN_NUMBER(GPIO_B_IDX, 3),
  PIN_PB4 = CREATE_PIN_NUMBER(GPIO_B_IDX, 4),
  PIN_PB5 = CREATE_PIN_NUMBER(GPIO_B_IDX, 5),
  PIN_PB6 = CREATE_PIN_NUMBER(GPIO_B_IDX, 6),
  PIN_PB7 = CREATE_PIN_NUMBER(GPIO_B_IDX, 7),
  PIN_PB8 = CREATE_PIN_NUMBER(GPIO_B_IDX, 8),
  PIN_PD0 = CREATE_PIN_NUMBER(GPIO_D_IDX, 0),
  PIN_PD1 = CREATE_PIN_NUMBER(GPIO_D_IDX, 1),

};
#elif LIBCH32_DEVICE_ID == WCH_CH32V003F4
/**
 * @brief Enumerate all valid port pins.
 *
 * Port idx & pin number are encoded in values
 */
enum GPIOPinId {
  PIN_PA1 = CREATE_PIN_NUMBER(GPIO_A_IDX, 1),
  PIN_PA2 = CREATE_PIN_NUMBER(GPIO_A_IDX, 2),
  PIN_PC1 = CREATE_PIN_NUMBER(GPIO_C_IDX, 1),
  PIN_PC2 = CREATE_PIN_NUMBER(GPIO_C_IDX, 2),
  PIN_PC3 = CREATE_PIN_NUMBER(GPIO_C_IDX, 3),
  PIN_PC4 = CREATE_PIN_NUMBER(GPIO_C_IDX, 4),
  PIN_PC5 = CREATE_PIN_NUMBER(GPIO_C_IDX, 5),
  PIN_PC6 = CREATE_PIN_NUMBER(GPIO_C_IDX, 6),
  PIN_PC7 = CREATE_PIN_NUMBER(GPIO_C_IDX, 7),
  PIN_PD0 = CREATE_PIN_NUMBER(GPIO_D_IDX, 0),
  PIN_PD1 = CREATE_PIN_NUMBER(GPIO_D_IDX, 1),
  PIN_PD2 = CREATE_PIN_NUMBER(GPIO_D_IDX, 2),
  PIN_PD3 = CREATE_PIN_NUMBER(GPIO_D_IDX, 3),
  PIN_PD4 = CREATE_PIN_NUMBER(GPIO_D_IDX, 4),
  PIN_PD5 = CREATE_PIN_NUMBER(GPIO_D_IDX, 5),
  PIN_PD6 = CREATE_PIN_NUMBER(GPIO_D_IDX, 6),
  PIN_PD7 = CREATE_PIN_NUMBER(GPIO_D_IDX, 7),
};
#else
#erorr "unsupported device"
#endif

/**
 * @brief Defines all possible valid combinations of Pin mode
 *
 * PU/PD & GPIO CFG CNFy,MODEy are encoded in each value
 * Eg 0b11000 --> PU=1 CNFy=10 MODEy=00
 */
typedef enum {

  PIN_MODE_INPUT_ANALOG = 0b0'00'00,
  PIN_MODE_INPUT_FLOATING = 0b0'01'00,

  PIN_MODE_INPUT_PULL_UP = 0b1'10'00,
  PIN_MODE_INPUT_PULL_DOWN = 0b0'10'00,

  PIN_MODE_OUTPUT_PUSH_PULL_10MHZ = 0b0'00'01,
  PIN_MODE_OUTPUT_PUSH_PULL_2MHZ = 0b0'00'10,
  PIN_MODE_OUTPUT_PUSH_PULL_50MHZ = 0b0'00'11,

  PIN_MODE_OUTPUT_OPEN_DRAIN_10MHZ = 0b0'01'01,
  PIN_MODE_OUTPUT_OPEN_DRAIN_2MHZ = 0b0'01'10,
  PIN_MODE_OUTPUT_OPEN_DRAIN_50MHZ = 0b0'01'11,

  PIN_MODE_ALTERNATE_FUNC_PUSH_PULL_10MHZ = 0b0'10'01,
  PIN_MODE_ALTERNATE_FUNC_PUSH_PULL_2MHZ = 0b0'10'10,
  PIN_MODE_ALTERNATE_FUNC_PUSH_PULL_50MHZ = 0b0'10'11,

  PIN_MODE_ALTERNATE_FUNC_OPEN_DRAIN_10MHZ = 0b0'11'01,
  PIN_MODE_ALTERNATE_FUNC_OPEN_DRAIN_2MHZ = 0b0'1'110,
  PIN_MODE_ALTERNATE_FUNC_OPEN_DRAIN_50MHZ = 0b0'11'11,
} GPIOPinMode;

/**
 * @brief Cache to hold a set of values to allow fast GPIO pin set/reset
 *
 */
struct GPIOPinSetCache {
  volatile uint32_t* bshr;
  volatile uint32_t* bcr;
  uint16_t pin_bit_map;
};

/** Prototypes */

/**
 * @brief Initilaise the specific GPIO pin
 *
 * @param pin_id Which pin
 * @param mode Mode to enable for pin
 */
extern void gpio_pin_init(enum GPIOPinId pin_id, GPIOPinMode mode);

/**
 * @brief Set the specified pin to val
 *
 * @param pin_id Which pin
 * @param val 1 or 0
 */
extern void gpio_pin_set(enum GPIOPinId pin_id, uint8_t val);

/**
 * @brief Cache the address and bit mask of a pin within a port
 *
 * @param pin_id Which pin
 * @param cache Which to cache
 */
extern void gpio_pin_cache(enum GPIOPinId pin_id, struct GPIOPinSetCache* cache);

/**
 * @brief Set the specified pin to val using a cached pin set
 *
 * @param cache Cached pin set info
 * @param val 1 or 0
 */
inline void gpio_pin_set_fast(struct GPIOPinSetCache* cache, uint8_t val) {
  if (val) {
    *cache->bshr = cache->pin_bit_map;
  } else {
    *cache->bcr = cache->pin_bit_map;
  }
}
