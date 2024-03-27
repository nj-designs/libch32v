/**
 * @file adc.c
 * @author Neil Johnson (nj.designs@protonmail.com)
 * @brief
 * @version 0.1
 * @date 2024-03-27
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "adc.h"

#ifdef LIBCH32_HAS_ADC1
struct ADCRegMap __attribute__((section(".adc1"))) adc1;
#endif

#ifdef LIBCH32_HAS_ADC2
struct ADCRegMap __attribute__((section(".adc2"))) adc2;
#endif
