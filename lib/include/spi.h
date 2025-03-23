/**
 * @file spi.h
 * @author Neil Johnson (nj.designs@protonmail.com)
 * @brief
 * @version 0.1
 * @date 2024-03-25
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once

#include <stdint.h>

struct SPIRegMap {
  volatile uint16_t ctlr1;  // 0x00
  uint16_t rsvd1;
  volatile uint16_t ctlr2;  // 0x04
  uint16_t rsvd2;
  volatile uint16_t statr;  // 0x08
  uint16_t rsvd3;
  volatile uint16_t datar;  // 0x0C
  uint16_t rsvd4;
  volatile uint16_t crcr;  // 0x10
  uint16_t rsvd5;
  volatile uint16_t rcrcr;  // 0x14
  uint16_t rsvd6;
  volatile uint16_t tcrcr;  // 0x18
  uint16_t rsvd7;
  volatile uint16_t i2s_cfgr;  // 0x1C
  volatile uint16_t i2sspr;    // 0x20 (Reserved for SPI1)
  volatile uint16_t hscr;      // 0x24
};

#ifdef LIBCH32_HAS_SPI1
extern struct SPIRegMap spi1;
#endif

// CTLR1
//------
// BIDIMODE[15]
static const uint16_t SPI_CTLR1_BIDIMODE = (1 << 15);
// BIDIOE[14]
static const uint16_t SPI_CTLR1_BIDIOE = (1 << 14);
// CRCEN[13]
static const uint16_t SPI_CTLR1_CRCEN = (1 << 13);
// CRCNEXT[12]
static const uint16_t SPI_CTLR1_CRCNEXT = (1 << 12);
// DFF[11]
static const uint16_t SPI_CTLR1_DFF = (1 << 11);
// RXONLY[10]
static const uint16_t SPI_CTLR1_RXONLY = (1 << 10);
// SSM[9]
static const uint16_t SPI_CTLR1_SSM = (1 << 9);
// SSI[8]
static const uint16_t SPI_CTLR1_SSI = (1 << 8);
// LSBFIRST[7]
static const uint16_t SPI_CTLR1_LSBFIRST = (1 << 7);
// SPE[6]
static const uint16_t SPI_CTLR1_SPE = (1 << 6);
// BR[5:3]
static const uint16_t SPI_CTLR1_BR_MASK = (0b111 << 3);
static const uint16_t SPI_CTLR1_BR_FPCLK_BY_2 = (0b000 << 3);
static const uint16_t SPI_CTLR1_BR_FPCLK_BY_4 = (0b001 << 3);
static const uint16_t SPI_CTLR1_BR_FPCLK_BY_8 = (0b010 << 3);
static const uint16_t SPI_CTLR1_BR_FPCLK_BY_16 = (0b011 << 3);
static const uint16_t SPI_CTLR1_BR_FPCLK_BY_32 = (0b100 << 3);
static const uint16_t SPI_CTLR1_BR_FPCLK_BY_64 = (0b101 << 3);
static const uint16_t SPI_CTLR1_BR_FPCLK_BY_128 = (0b110 << 3);
static const uint16_t SPI_CTLR1_BR_FPCLK_BY_256 = (0b111 << 3);
// MSTR[2]
static const uint16_t SPI_CTLR1_MSTR = (1 << 2);
// CPOL[1]
static const uint16_t SPI_CTLR1_CPOL = (1 << 1);
// CPHA[0]
static const uint16_t SPI_CTLR1_CPHA = (1 << 0);

// CTLR2
//------
// TXEIE[7]
static const uint16_t SPI_CTLR2_TXEIE = (1 << 7);
// RXNEIE[6]
static const uint16_t SPI_CTLR2_RXNEIE = (1 << 6);
// ERRIE[5]
static const uint16_t SPI_CTLR2_ERRIE = (1 << 5);
// SSOE[2]
static const uint16_t SPI_CTLR2_SSOE = (1 << 2);
// TXDMAEN[1]
static const uint16_t SPI_CTLR2_TXDMAEN = (1 << 1);
// RXDMAEN[0]
static const uint16_t SPI_CTLR2_RXDMAEN = (1 << 0);

// STATR
//------
// BSY[7]
static const uint16_t SPI_STATR_BSY = (1 << 7);
// OVR[6]
static const uint16_t SPI_STATR_OVR = (1 << 6);
// MODF[5]
static const uint16_t SPI_STATR_MODF = (1 << 5);
// CRCERR[4]
static const uint16_t SPI_STATR_CRCERR = (1 << 4);
// UDR[3]
static const uint16_t SPI_STATR_UDR = (1 << 3);
// CHSID[2]
static const uint16_t SPI_STATR_CHSID = (1 << 2);
// TXE[1]
static const uint16_t SPI_STATR_TXE = (1 << 1);
// RXNE[0]
static const uint16_t SPI_STATR_RXNE = (1 << 0);

// HSCR
// HSRXEN[0]
static const uint16_t SPI_HSCR_HSRXEN = (1 << 0);

/**
 * @brief Enumerate all of the SPI HW instances supported on this device
 *
 */
enum SPIInstanceId {
  SPI_INSTANCE1,
};

/**
 * @brief
 *
 */
struct SPICfgValues {
  uint16_t baud_rate;
  enum SPIInstanceId id;
};

void spi_cfg(struct SPICfgValues* cfg);
