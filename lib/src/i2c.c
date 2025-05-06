/**
 * @file i2c.c
 * @author Neil Johnson (nj.designs@protonmail.com)
 * @brief
 * @version 0.1
 * @date 2024-04-01
 *
 * @copyright Copyright (c) 2024
 *
 */
#include <stddef.h>

#include "i2c.h"
#include "rcc.h"

#include "printf.h"

#ifdef LIBCH32_HAS_I2C1
struct I2CRegMap __attribute__((section(".i2c1"))) i2c1;
#endif

#ifdef LIBCH32_HAS_I2C2
struct I2CRegMap __attribute__((section(".i2c2"))) i2c2;
#endif

#if defined(LIBCH32_V307_FAMILY)
static struct I2CRegMap *const i2c_reg_lookup[] = {
    &i2c1, // I2C_ID_1
    &i2c2, // I2C_ID_2
};
#elif LIBCH32_DEVICE_ID == WCH_CH32V203G6U6
static struct I2CRegMap *const i2c_reg_lookup[] = {
    &i2c1, // I2C_ID_1
    NULL,  // I2C_ID_2
};
#elif LIBCH32_DEVICE_ID == WCH_CH32V003F4
static struct I2CRegMap *const i2c_reg_lookup[] = {
    &i2c1, // I2C_ID_1
    NULL,  // I2C_ID_2
};
#else
#erorr "unsupported device"
#endif

void i2c_cfg(enum I2CId id, const struct I2CCfgValues *cfg) {
  struct I2CRegMap *reg = i2c_reg_lookup[(uint32_t)id];
  if (reg != nullptr) {
    uint16_t tmpreg, freq, result;

    rcc_reset_peripherial(id == I2C_ID_1 ? RCC_I2C1_ID : RCC_I2C2_ID);

    const uint32_t pclk1_freq = rcc_get_clk_freq(RCC_CLOCK_ID_PCLK1);
    tmpreg = reg->ctlr2 & ~I2C_CTRL2_FREQ_MASK;
    freq = (uint16_t)(pclk1_freq / 1'000'000);
    reg->ctlr2 = tmpreg | (freq & I2C_CTRL2_FREQ_MASK);
    if (freq >= 60) {
      freq = 60;
    }

    tmpreg = 0;

    if (cfg->bus_speed <= 100'000) {
      result = (uint16_t)(pclk1_freq / (cfg->bus_speed << 1));
      if (result < 4) {
        result = 4;
      }
      tmpreg |= result;
      reg->rtr = freq + 1;
    } else {
      if (cfg->duty_cycle == I2C_DUTY_CYCLE_2) {
        result = (uint16_t)(pclk1_freq / (cfg->bus_speed * 3));
      } else {
        result = (uint16_t)(pclk1_freq / (cfg->bus_speed * 25));
        result |= I2C_DUTY_CYCLE_16_9;
      }

      if ((result & I2C_CKCFGR_CCR_MASK) == 0) {
        result |= (uint16_t)0x0001;
      }
      tmpreg |= (uint16_t)(result | I2C_CKCFGR_FnS);
      reg->rtr = (uint16_t)(((freq * (uint16_t)300) / (uint16_t)1000) + (uint16_t)1);
    }

    reg->ckcfgr = tmpreg;

    if (cfg->ack) {
      reg->ctlr1 |= I2C_CTLR1_ACK;
    }
  }
};

void i2c_enable(enum I2CId id, uint32_t en) {
  struct I2CRegMap *reg = id < I2C_ID_MAX ? i2c_reg_lookup[(uint32_t)id] : nullptr;
  if (reg != nullptr) {
    if (en) {
      reg->ctlr1 |= I2C_CTLR1_PE;
    } else {
      reg->ctlr1 &= ~I2C_CTLR1_PE;
    }
  }
}

static bool check_status_flags(struct I2CRegMap *reg, uint16_t star1, uint16_t star2) {
  bool match1 = (reg->star1 & star1) == star1;
  bool match2 = (reg->star2 & star2) == star2;
  return match1 && match2;
}

static const uint32_t MAX_LOOP = 10'000;

int32_t i2c_connect(enum I2CId id, uint16_t target_address, enum I2CXferType xfer_type) {
  struct I2CRegMap *reg = id < I2C_ID_MAX ? i2c_reg_lookup[(uint32_t)id] : nullptr;
  if (reg != nullptr) {
    uint32_t cnt;

    // Start with clear status
    reg->star1 = 0;
    reg->star2 = 0;

    // Wait for !BUSY
    cnt = MAX_LOOP;
    while (cnt) {
      if (check_status_flags(reg, 0, I2C_STAR2_BUSY) == false) {
        break;
      }
      cnt--;
    }
    if (cnt == 0) {
      return -__LINE__;
    }

    reg->ctlr1 |= I2C_CTLR1_START;

    // Wait for master mode select
    cnt = MAX_LOOP;
    while (cnt) {
      if (check_status_flags(reg, I2C_STAR1_SB, I2C_STAR2_MSL)) {
        break;
      }
      cnt--;
    }
    if (cnt == 0) {
      return -__LINE__;
    }

    reg->datar = (target_address << 1) | (xfer_type == I2C_XFER_WRITE ? 0 : 1);

    cnt = MAX_LOOP;
    while (cnt) {
      if (check_status_flags(reg, I2C_STAR1_ADDR, 0)) {
        break;
      }
      cnt--;
    }
    if (cnt == 0) {
      // printf("star1:0x%04X star2:0x%04X\n", reg->star1, reg->star2);
      reg->ctlr1 |= I2C_CTLR1_STOP;
      return -__LINE__;
    }
    return 0;
  }
  return -__LINE__;
}

int32_t i2c_read(enum I2CId id, uint8_t *buffer, uint16_t max_read_len) {
  struct I2CRegMap *reg = id < I2C_ID_MAX ? i2c_reg_lookup[(uint32_t)id] : nullptr;
  if (reg != nullptr) {
    reg->ctlr1 |= I2C_CTLR1_ACK;
    uint16_t cnt;
    for (cnt = 0; cnt < max_read_len; cnt++) {
      while (check_status_flags(reg, I2C_STAR1_RxNE, 0) == 0) {
      }
      buffer[cnt] = (uint8_t)reg->datar;
    }
    return cnt;
  }
  return -__LINE__;
}

int32_t i2c_write(enum I2CId id, const uint8_t *buffer, uint16_t wr_len) {
  struct I2CRegMap *reg = id < I2C_ID_MAX ? i2c_reg_lookup[(uint32_t)id] : nullptr;
  if (reg != nullptr) {
    for (uint16_t cnt = 0; cnt < wr_len; cnt++) {
      while (check_status_flags(reg, I2C_STAR1_TxE, 0) == 0) {
      }
      reg->datar = buffer[cnt];
    }
  }
  return 0;
}

void i2c_disconnect(enum I2CId id) {
  struct I2CRegMap *reg = id < I2C_ID_MAX ? i2c_reg_lookup[(uint32_t)id] : nullptr;
  if (reg != nullptr) {
    reg->ctlr1 |= I2C_CTLR1_STOP;
  }
}
