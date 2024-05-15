/**
 * @file spi.c
 * @author Neil Johnson (nj.designs@protonmail.com)
 * @brief
 * @version 0.1
 * @date 2024-03-25
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "spi.h"

#ifdef LIBCH32_HAS_SPI1
struct SPIRegMap __attribute__((section(".spi1"))) spi1;
#endif

void spi_cfg(struct SPICfgValues* cfg) { (void)cfg; }