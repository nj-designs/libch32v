/**
 * @file dma.c
 * @author Neil Johnson (nj.designs@protonmail.com)
 * @brief
 * @version 0.1
 * @date 2024-03-23
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once

#include <stddef.h>
#include <stdint.h>

#include "core.h"
#include "dma.h"
#include "usart.h"

// TODO(njohn): Handle multiple DMA enginese?
struct DMA1RegMap __attribute__((section(".dma1"))) dma1;

static struct list_head per_channel_dma_req_q[DMA1_MAX_CHANNEL_COUNT];

/**
 * @brief Contains parameters pertaining to a specific DMA operation.
 *
 */
struct DMAPeripheralLookup {
  uint32_t        reg_addr;
  uint32_t        cfgr;
  uint32_t        intfcr;
  enum PFICIntNum pfic_int_num;
  uint8_t         chan_idx;
};

/**
 * @brief Lookup of periheral mapping
 *
 * Note: Must match DMAPeripheralId in order and size
 *
 */
static const struct DMAPeripheralLookup peripheral_lookup[] = {
    // DMA_PERIPHERAL_ID_USART1_TX,
    {
        .reg_addr = (uint32_t)&usart1.datar,
        .cfgr = DMA_CHANNEL_CFGR_MSIZE_8_BITS | DMA_CHANNEL_CFGR_PSIZE_16_BITS | DMA_CHANNEL_CFGR_MINC |
                DMA_CHANNEL_CFGR_DIR | DMA_CHANNEL_CFGR_TEIE | DMA_CHANNEL_CFGR_TCIE,
        .intfcr = DMA_INTFCR_CTEIF4 | DMA_INTFCR_HTIF4 | DMA_INTFCR_TCIF4 | DMA_INTFCR_GIF4,
        .chan_idx = 3, // Idx = channel number - 1
        .pfic_int_num = PFIC_DMA1_CH4_INT_NUM,
    },
};

/**
 * @brief program the dma engine for the supplied req
 *
 * Note: Currently only supports DMA1
 *
 * @param req
 */
static void program_dma_request(struct DMAXferRequest *req) {
  struct DMAChanelRegMap *creg = &dma1.channel[req->_chan_idx];
  dma1.intfcr = req->_intfcr;
  creg->cfgr = req->_cfgr;
  creg->paddr = req->_peripheral_addr;
  creg->maddr = (uint32_t)req->memory_address;
  creg->cntr = req->xfter_len;
  creg->cfgr |= DMA_CHANNEL_CFGR_EN;
}

void dma_queue_xfer_request(struct DMAXferRequest *req) {
  const struct DMAPeripheralLookup *pl = &peripheral_lookup[(uint32_t)req->id];

  req->_cfgr = pl->cfgr;
  req->_peripheral_addr = pl->reg_addr;
  req->_intfcr = pl->intfcr;
  req->_chan_idx = pl->chan_idx;

  core_disable_pfic_irq(pl->pfic_int_num);

  struct list_head *req_list = &per_channel_dma_req_q[req->_chan_idx];
  list_insert_node_after(req_list, NULL, (struct list_node *)req);
  if (list_head(req_list) == req) {
    program_dma_request(req);
  }

  core_enable_pfic_irq(pl->pfic_int_num);
}

/**
 * @brief Interrupt handler for DMA1 channel 4
 *
 */
void int_handler_dma1_chan4(void) NJD_IRQ_ATTRIBUTE;
void int_handler_dma1_chan4(void) {
  struct list_head *req_list = &per_channel_dma_req_q[3];

  if (dma1.intfr & DMA_INTFR_GIF4) {
    // TODO(njohn) : Check for dma errors?
    dma1.intfcr |= DMA_INTFR_TEIF4 | DMA_INTFR_HTIF4 | DMA_INTFR_TCIF4 | DMA_INTFR_GIF4;
    struct DMAXferRequest *req = (struct DMAXferRequest *)list_head(req_list);
    if (req != NULL) {
      if (req->cb != NULL) {
        req->cb(req);
      }
      list_remove_node(req_list, (struct list_node *)req);
      req = (struct DMAXferRequest *)list_head(req_list);
      if (req != NULL) {
        program_dma_request(req);
      }
    }
  }
}
