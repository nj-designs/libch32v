/**
 * @file ds3231.h
 * @author Neil Johnson (neil@hpe.com)
 * @brief
 * @version 0.1
 * @date 2024-04-16
 *
 * @copyright Copyright 2024 Hewlett Packard Enterprise Development LP.
 *
 */
#pragma once

#include <stdint.h>

#include "i2c.h"

static const uint16_t DS3231_DEFAULT_I2C_ADDR = 0x68;  // < The default i2c address of DS3231

static constexpr uint32_t DS3231_REG_COUNT = 0x13;

/**
 * @brief Instance of a connected DS3231
 *
 */
struct DS3231 {
  uint16_t i2c_addr_;
  enum I2CId i2c_id_;
  uint8_t regs_[DS3231_REG_COUNT];
};

struct DS3231DateTime {
  uint8_t secs;
  uint8_t mins;
  uint8_t hours;
  uint8_t day_of_month;
  uint8_t months;
  uint8_t years;
  uint8_t day_of_week;
};

/**
 * @brief
 *
 * @param i2c_id
 * @param i2c_addr
 * @param ins
 */
void ds3231_init(enum I2CId i2c_id, uint16_t i2c_addr, struct DS3231* ins);

/**
 * @brief Refresh the internal reg buffer from the device
 *
 * @param ins
 * @return int32_t
 */
int32_t ds3231_refresh(struct DS3231* ins);

/**
 * @brief Populate the supplied data time structure.
 *
 * @param ins DS3231 instanct
 * @param dt data time structure to populate
 * @param refresh refresh values first
 * @return int32_t
 */
int32_t ds3231_get_date_time(struct DS3231* ins, struct DS3231DateTime* dt, bool refresh);
