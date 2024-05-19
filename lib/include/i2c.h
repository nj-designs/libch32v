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

#include <stdint.h>
#include <stdbool.h>

struct I2CRegMap {
  volatile uint16_t ctlr1;
  uint16_t rsvd1;
  volatile uint16_t ctlr2;
  uint16_t rsvd12;
  volatile uint16_t oaddr1;
  uint16_t rsvd3;
  volatile uint16_t oaddr2;
  uint16_t rsvd4;
  volatile uint16_t datar;
  uint16_t rsvd5;
  volatile uint16_t star1;
  uint16_t rsvd6;
  volatile uint16_t star2;
  uint16_t rsvd7;
  volatile uint16_t ckcfgr;
  uint16_t rsvd8;
  volatile uint16_t rtr;
};

#ifdef LIBCH32_HAS_I2C1
extern struct I2CRegMap i2c1;
#endif

#ifdef LIBCH32_HAS_I2C2
extern struct I2CRegMap i2c1;
#endif

// CTLR1
//------
// SWRST[15]
static const uint16_t I2C_CTLR1_SWRST = (1 << 15);
// ACK[10]
static const uint16_t I2C_CTLR1_ACK = (1 << 10);
// STOP[9]
static const uint16_t I2C_CTLR1_STOP = (1 << 9);
// START[8]
static const uint16_t I2C_CTLR1_START = (1 << 8);
// PE[0]
static const uint16_t I2C_CTLR1_PE = (1 << 0);

// CTRL2
//------
// FREQ[5:0]
static const uint16_t I2C_CTRL2_FREQ_MASK = (0b111111 << 0);

// OADDR1
//-------
// ADDMODE[15]
static const uint16_t I2C_OADDR1_ADDMODE = (1 << 15);

// STAR1
//------
// AF[10]
static const uint16_t I2C_STAR1_AF = (1 << 10);
// TxE[7]
static const uint16_t I2C_STAR1_TxE = (1 << 7);
// RxNE[6]
static const uint16_t I2C_STAR1_RxNE = (1 << 6);
// ADDR[1]
static const uint16_t I2C_STAR1_ADDR = (1 << 1);
// SB[0]
static const uint16_t I2C_STAR1_SB = (1 << 0);

// STAR2
//-------
// TRA[2]
static const uint16_t I2C_STAR2_TRA = (1 << 2);
// BUSY[1]
static const uint16_t I2C_STAR2_BUSY = (1 << 1);
// MSL[0]
static const uint16_t I2C_STAR2_MSL = (1 << 0);

// CKCFGR
//-------
// FnS[15]
static const uint16_t I2C_CKCFGR_FnS = (1 << 15);
// DUTY[14]
static const uint16_t I2C_CKCFGR_DUTY = (1 << 14);
// CCR[11:0]
static const uint16_t I2C_CKCFGR_CCR_MASK = (0x0FFF << 0);

enum I2CId { I2C_ID_1, I2C_ID_2, I2C_ID_MAX };

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

void i2c_cfg(enum I2CId id, const struct I2CCfgValues* cfg);

void i2c_start(enum I2CId id);

/**
 * @brief Connect to a target address
 *
 * @param id
 * @param target_address
 * @param xfer_type
 *
 */
int32_t i2c_connect(enum I2CId id, uint16_t target_address, enum I2CXferType xfer_type);

/**
 * @brief
 *
 * @param id
 * @param buffer
 * @param max_read_len
 * @return int32_t
 */
int32_t i2c_read(enum I2CId id, uint8_t* buffer, uint16_t max_read_len);

int32_t i2c_write(enum I2CId id, const uint8_t* buffer, uint16_t wr_len);

/**
 * @brief
 *
 * @param id
 */
void i2c_disconnect(enum I2CId id);

void i2c_enable(enum I2CId id, uint32_t en);