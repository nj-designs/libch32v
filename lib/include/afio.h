/**
 * @file afio.h
 * @author Neil Johnson (nj.designs@protonmail.com)
 * @brief
 * @version 0.1
 * @date 2024-06-02
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once

#include <stdint.h>

#include "rcc.h"

#if defined(LIBCH32_V307_FAMILY)
// PCFR1
// -----
// PD01_RM[15]
static const uint32_t AFIO_PCFR1_PD01_RM_XTAL = 0;
static const uint32_t AFIO_PCFR1_PD01_RM_GPIO = 1;
// CAN1_RM[14:13]
static const uint32_t AFIO_PCFR1_CAN1_RM_PA11_PA12 = 0b00;
static const uint32_t AFIO_PCFR1_CAN1_RM_PB8_PB9   = 0b01;
static const uint32_t AFIO_PCFR1_CAN1_RM_PD0_PD1   = 0b11;
// TIM2_RM[9:8]
static const uint32_t AFIO_PCFR1_TIM2_RM_DEFAULT    = 0b00;
static const uint32_t AFIO_PCFR1_TIM2_RM_PARTIAL_01 = 0b01;
static const uint32_t AFIO_PCFR1_TIM2_RM_PARTIAL_10 = 0b10;
static const uint32_t AFIO_PCFR1_TIM2_RM_FULLY      = 0b11;
// TIM1_RM[7:6]
static const uint32_t AFIO_PCFR1_TIM1_RM_DEFAULT    = 0b00;
static const uint32_t AFIO_PCFR1_TIM1_RM_PARTIAL_01 = 0b01;
static const uint32_t AFIO_PCFR1_TIM1_RM_PARTIAL_10 = 0b10;
static const uint32_t AFIO_PCFR1_TIM1_RM_FULLY      = 0b11;
#else
#error unsupported
#endif

void afio_remap(enum RCCPeripheralId id, uint32_t rmv);
