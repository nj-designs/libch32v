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

// TODO(njohn): Fix up for V203
struct AFIORegMap {
  volatile uint32_t ecr;
  volatile uint32_t pcfr1;
  volatile uint32_t exticr;
};

extern struct AFIORegMap afio;

// PCFR1
// -----
// PA12_RM[15]
static const uint32_t AFIO_PCFR1_PA12_RM = (1 << 15);
// TIM2RM[9:8]
static const uint32_t AFIO_PCFR1_TIM2RM_MASK = (0b11 << 8);
static const uint32_t AFIO_PCFR1_TIM2RM_DEFAULT = (0b00 << 8);
static const uint32_t AFIO_PCFR1_TIM2RM_PARTIAL_01 = (0b01 << 8);
static const uint32_t AFIO_PCFR1_TIM2RM_PARTIAL_10 = (0b10 << 8);
static const uint32_t AFIO_PCFR1_TIM2RM_COMPLETE = (0b11 << 8);
// TIM1RM[7:6]
static const uint32_t AFIO_PCFR1_TIM1RM_MASK = (0b11 << 6);
static const uint32_t AFIO_PCFR1_TIM1RM_DEFAULT = (0b00 << 6);
static const uint32_t AFIO_PCFR1_TIM1RM_PARTIAL_01 = (0b01 << 6);
static const uint32_t AFIO_PCFR1_TIM1RM_PARTIAL_10 = (0b10 << 6);
static const uint32_t AFIO_PCFR1_TIM1RM_COMPLETE = (0b11 << 6);
