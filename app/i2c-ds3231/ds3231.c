/**
 * @file ds3231.c
 * @author Neil Johnson (neil@hpe.com)
 * @brief
 * @version 0.1
 * @date 2024-04-16
 *
 * @copyright Copyright 2024 Hewlett Packard Enterprise Development LP.
 *
 */
#include "ds3231.h"

static inline uint8_t ds3231_parse_seconds(uint8_t b) { return (((b >> 4) & 0b111) * 10) + (b & 0b1111); }

static inline uint8_t ds3231_parse_minutes(uint8_t b) { return (((b >> 4) & 0b111) * 10) + (b & 0b1111); }

/**
 * @brief Always return time in 24hr format
 *
 * @param b
 * @return uint8_t
 */
static inline uint8_t ds3231_parse_hours(uint8_t b) {
  uint8_t uh, th;

  if ((b & (1 << 6)) == 0) {
    // 24h
    uh = b & 0b1111;
    th = (b >> 4) & 0b11;
    return th * 10 + uh;
  } else {
    // !24hr
    uh = b & 0b1111;
    th = (b >> 4) & 0b01;
    return th * 10 + uh + 12;
  }
}

void ds3231_init(enum I2CId i2c_id, uint16_t i2c_addr, struct DS3231* ins) {
  ins->i2c_id_ = i2c_id;
  ins->i2c_addr_ = i2c_addr;
}

int32_t ds3231_refresh(struct DS3231* ins) {
  int32_t res;
  uint8_t buffer[1];

  // Write zero to reset reg ptr back to reg set
  res = i2c_connect(ins->i2c_id_, ins->i2c_addr_, I2C_XFER_WRITE);
  if (res < 0) {
    return res;
  }
  buffer[0] = 0;
  res = i2c_write(ins->i2c_id_, buffer, 1);
  i2c_disconnect(ins->i2c_id_);
  if (res < 0) {
    return res;
  }
  i2c_disconnect(ins->i2c_id_);

  // Read whole reg set
  res = i2c_connect(ins->i2c_id_, ins->i2c_addr_, I2C_XFER_READ);
  if (res < 0) {
    return res;
  }
  res = i2c_read(ins->i2c_id_, ins->regs_, DS3231_REG_COUNT);
  i2c_disconnect(ins->i2c_id_);
  if (res < 0) {
    return res;
  }
  return 0;
}

int32_t ds3231_get_date_time(struct DS3231* ins, struct DS3231DateTime* dt, bool refresh) {
  int32_t res;

  if (refresh) {
    res = ds3231_refresh(ins);
    if (res < 0) {
      return res;
    }
  }

  dt->secs = ds3231_parse_seconds(ins->regs_[0]);
  dt->mins = ds3231_parse_minutes(ins->regs_[1]);
  dt->hours = ds3231_parse_hours(ins->regs_[2]);

  return 0;
}
