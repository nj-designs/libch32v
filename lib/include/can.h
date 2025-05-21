/**
 * @file can.h
 * @author Neil Johnson (nj.designs@protonmail.com)
 * @brief
 * @version 0.1
 * @date 2025-04-16
 *
 * @copyright Copyright (c) 2025
 *
 */
#pragma once

#if !defined(LIBCH32_HAS_CAN1)
#error This device doesn't support CAN
#endif

#include <stdint.h>
#include <sys/types.h>

#define CAN_EXT_BIT ((uint32_t)(1 << 31))
#define CAN_STD_ID(_id) ((uint32_t)(_id & 0x7ff))
#define CAN_EXT_ID(_id) (CAN_EXT_BIT | (uint32_t)(_id & 0x1fffffff))

enum CanCtrlId {
  CAN_CTRL_ID_1,
#if defined(LIBCH32_HAS_CAN2)
  CAN_CTRL_ID_2,
#endif
};

struct CANTxReq {
  enum CanCtrlId ctrl_id;
  uint32_t id;
  const uint8_t *data_ptr;
  uint8_t data_len;
  // Private
  uint8_t _mb_idx;
};

typedef struct CANTxReq CanRxMsg;

typedef void (*can_rx_cb)(const CanRxMsg *can_msg);

enum CanTxStatus {

  CAN_TX_DONE,
  CAN_TX_ERROR,
  CAN_TX_RUNNING
};

void can_init(enum CanCtrlId block_id, uint32_t bus_speed, bool silent,
              bool loopback, can_rx_cb rx_cb);
void can_deinit(enum CanCtrlId block_id);
void can_filter_init(void);
bool can_tx_req(struct CANTxReq *req, uint32_t max_wait_ms);
enum CanTxStatus can_check_tx_complete(const struct CANTxReq *req);
void can_filter_init_ex(enum CanCtrlId block_id, const uint32_t *ids,
                        uint32_t id_cnt);
