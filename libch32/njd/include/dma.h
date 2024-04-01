/**
 * @file dma.h
 * @author Neil Johnson (nj.designs@protonmail.com)
 * @brief
 * @version 0.1
 * @date 2024-03-23
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once

#include <stdint.h>

#include "list_mgr.h"

/**
 * @brief Per DMA channel register map
 *
 */
struct DMAChanelRegMap {
  volatile uint32_t cfgr;
  volatile uint32_t cntr;
  volatile uint32_t paddr;
  volatile uint32_t maddr;
  uint32_t rsvd1;
};

// CFGR
//-----
// MEM2MEM[14]
static const uint32_t DMA_CHANNEL_CFGR_MEM2MEM = (1 << 14);
// PL[13:12]
static const uint32_t DMA_CHANNEL_CFGR_PL_MASK = (0b00 << 12);
static const uint32_t DMA_CHANNEL_CFGR_PL_LOW = (0b00 << 12);
static const uint32_t DMA_CHANNEL_CFGR_PL_MEDIUM = (0b01 << 12);
static const uint32_t DMA_CHANNEL_CFGR_PL_HIGH = (0b10 << 12);
static const uint32_t DMA_CHANNEL_CFGR_PL_VERY_HIGH = (0b11 << 12);
// MSIZE[11:10]
static const uint32_t DMA_CHANNEL_CFGR_MSIZE_MASK = (0b11 << 10);
static const uint32_t DMA_CHANNEL_CFGR_MSIZE_8_BITS = (0b00 << 10);
static const uint32_t DMA_CHANNEL_CFGR_MSIZE_16_BITS = (0b01 << 10);
static const uint32_t DMA_CHANNEL_CFGR_MSIZE_32_BITS = (0b10 << 10);
// PSIZE[9:8]
static const uint32_t DMA_CHANNEL_CFGR_PSIZE_MASK = (0b11 << 8);
static const uint32_t DMA_CHANNEL_CFGR_PSIZE_8_BITS = (0b00 << 8);
static const uint32_t DMA_CHANNEL_CFGR_PSIZE_16_BITS = (0b01 << 8);
static const uint32_t DMA_CHANNEL_CFGR_PSIZE_32_BITS = (0b10 << 8);
// MINC[7]
static const uint32_t DMA_CHANNEL_CFGR_MINC = (1 << 7);
// PINC[6]
static const uint32_t DMA_CHANNEL_CFGR_PINC = (1 << 6);
// CIRC[5]
static const uint32_t DMA_CHANNEL_CFGR_CIRC = (1 << 5);
// DIR[4]
static const uint32_t DMA_CHANNEL_CFGR_DIR = (1 << 4);
// TEIE[3]
static const uint32_t DMA_CHANNEL_CFGR_TEIE = (1 << 3);
// HTIE[2]
static const uint32_t DMA_CHANNEL_CFGR_HTIE = (1 << 2);
// TCIE[1]
static const uint32_t DMA_CHANNEL_CFGR_TCIE = (1 << 1);
// EN[0]
static const uint32_t DMA_CHANNEL_CFGR_EN = (1 << 0);

// CNTR
//-----
// NDT[15:0]
static const uint32_t DMA_CHANNEL_CNTR_NDT_MASK = (0xFFFF);

#define DMA1_MAX_CHANNEL_COUNT (8)
/**
 * @brief DMA1 unit register map
 *
 */
struct DMA1RegMap {
  volatile uint32_t intfr;
  volatile uint32_t intfcr;
  struct DMAChanelRegMap channel[DMA1_MAX_CHANNEL_COUNT];
};
extern struct DMA1RegMap dma1;

// INTFR
//------
// TEIF8[31]
static const uint32_t DMA_INTFR_TEIF8 = (1 << 31);
// HTIF8[30]
static const uint32_t DMA_INTFR_HTIF8 = (1 << 30);
// TCIF8[29]
static const uint32_t DMA_INTFR_TCIF8 = (1 << 29);
// GIF8[28]
static const uint32_t DMA_INTFR_GIF8 = (1 << 28);
// TEIF7[27]
static const uint32_t DMA_INTFR_TEIF7 = (1 << 27);
// HTIF7[26]
static const uint32_t DMA_INTFR_HTIF7 = (1 << 26);
// TCIF7[25]
static const uint32_t DMA_INTFR_TCIF7 = (1 << 25);
// GIF7[24]
static const uint32_t DMA_INTFR_GIF7 = (1 << 24);
// TEIF6[23]
static const uint32_t DMA_INTFR_TEIFR6 = (1 << 23);
// HTIF6[22]
static const uint32_t DMA_INTFR_HTIF6 = (1 << 22);
// TCIF6[21]
static const uint32_t DMA_INTFR_TCIF6 = (1 << 21);
// GIF6[20]
static const uint32_t DMA_INTFR_GIF6 = (1 << 20);
// TEIF5[19]
static const uint32_t DMA_INTFR_TEIF5 = (1 << 19);
// HTIF5[18]
static const uint32_t DMA_INTFR_HTIF5 = (1 << 18);
// TCIF5[17]
static const uint32_t DMA_INTFR_TCIF5 = (1 << 17);
// GIF5[16]
static const uint32_t DMA_INTFR_GIF5 = (1 << 16);
// TEIF4[15]
static const uint32_t DMA_INTFR_TEIF4 = (1 << 15);
// HTIF4[14]
static const uint32_t DMA_INTFR_HTIF4 = (1 << 14);
// TCIF4[13]
static const uint32_t DMA_INTFR_TCIF4 = (1 << 13);
// GIF4[12]
static const uint32_t DMA_INTFR_GIF4 = (1 << 12);
// TEIF3[11]
static const uint32_t DMA_INTFR_TEIF3 = (1 << 11);
// HTIF3[10]
static const uint32_t DMA_INTFR_HTIF3 = (1 << 10);
// TCIF3[9]
static const uint32_t DMA_INTFR_TCIF3 = (1 << 9);
// GIF3[8]
static const uint32_t DMA_INTFR_GIF3 = (1 << 8);
// TEIF2[7]
static const uint32_t DMA_INTFR_TEIF2 = (1 << 7);
// HTIF2[6]
static const uint32_t DMA_INTFR_HTIF2 = (1 << 6);
// TCIF2[5]DMA_
static const uint32_t DMA_INTFR_TCIF2 = (1 << 5);
// GIF2[4]
static const uint32_t DMA_INTFR_GIF2 = (1 << 4);
// TEIF1[3]
static const uint32_t DMA_INTFR_TEIF1 = (1 << 3);
// HTIF1[2]
static const uint32_t DMA_INTFR_HTIF1 = (1 << 2);
// TCIF1[1]
static const uint32_t DMA_INTFR_TCIF1 = (1 << 1);
// GIF1[0]
static const uint32_t DMA_INTFR_GIF1 = (1 << 0);

// INTFCR
//-------
// CTEIF8[31]
static const uint32_t DMA_INTFCR_CTEIF8 = (1 << 31);
// CHTIF8[30]
static const uint32_t DMA_INTFCR_HTIF8 = (1 << 30);
// CTCIF8[29]
static const uint32_t DMA_INTFCR_TCIF8 = (1 << 29);
// CGIF8[28]
static const uint32_t DMA_INTFCR_GIF8 = (1 << 28);
// CTEIF7[27]
static const uint32_t DMA_INTFCR_CTEIF7 = (1 << 27);
// CHTIF7[26]
static const uint32_t DMA_INTFCR_HTIF7 = (1 << 26);
// CTCIF7[25]
static const uint32_t DMA_INTFCR_TCIF7 = (1 << 25);
// CGIF7[24]
static const uint32_t DMA_INTFCR_GIF7 = (1 << 24);
// CTEIF6[23]
static const uint32_t DMA_INTFCR_CTEIF6 = (1 << 23);
// CHTIF6[22]
static const uint32_t DMA_INTFCR_HTIF6 = (1 << 22);
// CTCIF6[21]
static const uint32_t DMA_INTFCR_TCIF6 = (1 << 21);
// CGIF6[20]
static const uint32_t DMA_INTFCR_GIF6 = (1 << 20);
// CTEIF5[19]
static const uint32_t DMA_INTFCR_CTEIF5 = (1 << 19);
// CHTIF5[18]
static const uint32_t DMA_INTFCR_HTIF5 = (1 << 18);
// CTCIF5[17]
static const uint32_t DMA_INTFCR_TCIF5 = (1 << 17);
// CGIF5[16]
static const uint32_t DMA_INTFCR_GIF5 = (1 << 16);
// CTEIF4[15]
static const uint32_t DMA_INTFCR_CTEIF4 = (1 << 15);
// CHTIF4[14]
static const uint32_t DMA_INTFCR_HTIF4 = (1 << 14);
// CTCIF4[13]
static const uint32_t DMA_INTFCR_TCIF4 = (1 << 13);
// CGIF4[12]
static const uint32_t DMA_INTFCR_GIF4 = (1 << 12);
// CTEIF3[11]
static const uint32_t DMA_INTFCR_CTEIF3 = (1 << 11);
// CHTIF3[10]
static const uint32_t DMA_INTFCR_HTIF3 = (1 << 10);
// CTCIF3[9]
static const uint32_t DMA_INTFCR_TCIF3 = (1 << 9);
// CGIF3[8]
static const uint32_t DMA_INTFCR_GIF3 = (1 << 8);
// CTEIF2[7]
static const uint32_t DMA_INTFCR_CTEIF2 = (1 << 7);
// CHTIF2[6]
static const uint32_t DMA_INTFCR_HTIF2 = (1 << 6);
// CTCIF2[5]
static const uint32_t DMA_INTFCR_TCIF2 = (1 << 5);
// CGIF2[4]
static const uint32_t DMA_INTFCR_GIF2 = (1 << 4);
// CTEIF1[3]
static const uint32_t DMA_INTFCR_CTEIF1 = (1 << 3);
// CHTIF1[2]
static const uint32_t DMA_INTFCR_HTIF1 = (1 << 2);
// CTCIF1[1]
static const uint32_t DMA_INTFCR_TCIF1 = (1 << 1);
// CGIF1[0]
static const uint32_t DMA_INTFCR_GIF1 = (1 << 0);

/**
 * @brief Forward declare
 *
 */
struct DMAXferRequest;

/**
 * @brief Function to be called upon completion of DMA Request.
 *
 * Note: Will be called from an interrupt
 *
 */
typedef void (*DMAXferCompleteCB)(struct DMAXferRequest* req);

/**
 * @brief Enumerate all support DMA'able peripherals
 *
 */
enum DMAPeripheralId {
  DMA_PERIPHERAL_ID_USART1_TX,
};

/**
 * @brief Defines a type that contains all details of a required DMA request
 *
 */
struct DMAXferRequest {
  struct list_node _node;    /**< Must be first item*/
  uint32_t arg;              /**< Caller supplied and used argument*/
  DMAXferCompleteCB cb;      /**< completion cb, or NULL for not required*/
  void* memory_address;      /**> Start of memory to wr to/read from*/
  uint32_t _cfgr;            /**> Precalculated value to write to cfgr reg (dma engine
                                private)*/
  uint32_t _peripheral_addr; /**> Pecalculated address of paddr (dma engine
                                private)*/
  uint32_t _intfcr;          /**> Pecalculated intfcr value (dma engine private)*/
  uint16_t xfter_len;        /**> Xfer size in bytes*/
  uint32_t _chan_idx;        /**> Stored chan idx (dma engine private)*/
  enum DMAPeripheralId id;   /**> Which peripheral to wr to/read from*/
};

/**
 * Prototypes from here down
 */

/**
 * @brief
 *
 * @param req
 */
void dma_queue_xfer_request(struct DMAXferRequest* req);