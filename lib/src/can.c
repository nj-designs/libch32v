/**
 * @file can.c
 * @author Neil Johnson (nj.designs@protonmail.com)
 * @brief
 * @version 0.1
 * @date 2025-04-16
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <stdint.h>

#include "can.h"


#ifdef LIBCH32_HAS_CAN1
struct CANRegMap __attribute__((section(".can1")))can1;
struct CANMailboxRegMap __attribute__((section(".can1_mb")))can1_mb;
struct CANFilterRegMap __attribute__((section(".can1_filter"))) can1_filter;
#endif
