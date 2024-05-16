
/**
 * @file init.c
 * @author Neil Johnson (nj.designs@protonmail.com)
 * @brief
 * @version 0.1
 * @date 2024-03-22
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "rcc.h"
#include "stdout.h"

#include "device_config.h"

void init_system(void) {
  rcc_init();

#if LIBCH32_DEVICE_ID == WCH_CH32V203G6U6
  rcc_cfg_clock_tree(LIBCH32_HSE_FREQ, LIBCH32_SYSCLK_FREQ);
#endif

#ifdef APP_STDOUT_BUFFER_SIZE
  stdout_init();
#endif
}