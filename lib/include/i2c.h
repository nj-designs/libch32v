/**
 * @file i2c.h
 * @author Neil Johnson (nj.designs@protonmail.com)
 * @brief
 * @version 0.1
 * @date 2024-04-01
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "libch32v-config.h"

enum I2CId {
  I2C_ID_1,
#if defined(LIBCH32_HAS_I2C2)
  I2C_ID_2
#endif
};

enum I2CDutyCycle {
  I2C_DUTY_CYCLE_16_9 = 0x4000,
  I2C_DUTY_CYCLE_2 = 0xBFFF,
};

enum I2CAckAddr {
  I2C_ACK_ADDR_7_BIT = 0x4000,
  I2C_ACK_ADDR_10_BIT = 0xC000,
};
struct I2CCfgValues {
  uint32_t bus_speed;
  enum I2CDutyCycle duty_cycle;
  bool ack;
  enum I2CAckAddr ack_addr;
};

enum I2CXferType { I2C_XFER_WRITE, I2C_XFER_READ };

void i2c_cfg(enum I2CId id, const struct I2CCfgValues *cfg);

void i2c_start(enum I2CId id);

/**
 * @brief Connect to a target address
 *
 * @param id
 * @param target_address
 * @param xfer_type
 *
 */
int32_t i2c_connect(enum I2CId id, uint16_t target_address,
                    enum I2CXferType xfer_type);

/**
 * @brief
 *
 * @param id
 * @param buffer
 * @param max_read_len
 * @return int32_t
 */
int32_t i2c_read(enum I2CId id, uint8_t *buffer, uint16_t max_read_len);

int32_t i2c_write(enum I2CId id, const uint8_t *buffer, uint16_t wr_len);

/**
 * @brief
 *
 * @param id
 */
void i2c_disconnect(enum I2CId id);

void i2c_enable(enum I2CId id, uint32_t en);
