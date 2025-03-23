/**
 * @file adtm.c
 * @author Neil Johnson (nj.designs@protonmail.com)
 * @brief
 * @version 0.1
 * @date 2025-03-16
 *
 * @copyright Copyright (c) 2025
 *
 */
#include "adtm.h"

#ifdef LIBCH32_HAS_ADTM1
struct ADTMRegMap __attribute__((section(".adtm1"))) adtm1;
#endif
