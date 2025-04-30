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

void init_system(void) {
  rcc_init();

#if defined(APP_HSE_FREQ) && defined(APP_SYSCLK_FREQ)
  rcc_cfg_clock_tree(APP_HSE_FREQ, APP_SYSCLK_FREQ);
#endif

#ifdef APP_STDOUT_BUFFER_SIZE
  stdout_init();
#endif
}
