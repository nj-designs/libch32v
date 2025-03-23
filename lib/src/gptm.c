/**
 * @file gptm.c
 * @author Neil Johnson (nj.designs@protonmail.com)
 * @brief
 * @version 0.1
 * @date 2025-03-21
 *
 * @copyright Copyright (c) 2025
 *
 */
#include "gptm.h"

#ifdef LIBCH32_HAS_GPTM2
struct GPTMRegMap __attribute__((section(".gptm2"))) gptm2;
#endif

#ifdef LIBCH32_HAS_GPTM3
struct GPTMRegMap __attribute__((section(".gptm3"))) gptm3;
#endif

#ifdef LIBCH32_HAS_GPTM4
struct GPTMRegMap __attribute__((section(".gptm4"))) gptm4;
#endif
