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
#include "core.h"
#include "rcc.h"

void init_system(void) {
  core_init();
  rcc_init();
}
