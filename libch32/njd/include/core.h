/**
 * @file core.h
 * @author Neil Johnson (nj.designs@protonmail.com)
 * @brief
 * @version 0.1
 * @date 2024-03-19
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once

#include <stdint.h>

#include "device_config.h"

/**
 * @brief System Tick Register Map
 *
 */
struct SysTickRegMap {
  volatile uint32_t ctrl;
  volatile uint32_t sr;
  volatile uint64_t cnt;
  volatile uint64_t cmp;
};

extern struct SysTickRegMap systick;

// STK_CTLR
//---------
// SWIE[31]
// static const uint32_t STK_CTLR_SWIE = (1 << 31);
// INIT[5]
static const uint32_t STK_CTLR_INIT = (1 << 5);
// MODE[4]
static const uint32_t STK_CTLR_MODE = (1 << 4);
// STE[0]
static const uint32_t STK_CTLR_STE = (1 << 0);

// STK_SR
//-------
// CNTIF
static const uint32_t STK_SR_CNTIF = (1 << 0);

/**
 * @brief PFIC Register map
 *
 */
struct PFICRegMap {
  volatile uint32_t isr[4];       // 0xE000E000
  volatile uint32_t rsvd1[4];     // 0xE000E010
  volatile uint32_t ipr[4];       // 0xE000E020
  volatile uint32_t rsvd2[4];     // 0xE000E030
  volatile uint32_t ithresdr;     // 0xE000E040
  volatile uint32_t rsvd3;        // 0xE000E044
  volatile uint32_t cfgr;         // 0xE000E048
  volatile uint32_t gisr;         // 0xE000E04C
  volatile uint32_t vtfidr;       // 0xE000E050
  volatile uint32_t rsvd4[3];     // 0xE000E054
  volatile uint32_t vtfaddrr[4];  // 0xE000E060
  volatile uint32_t rsvd5[36];    // 0xE000E070
  volatile uint32_t ienr[4];      // 0xE000E100
  volatile uint32_t rsvd6[28];    // 0xE000E110
  volatile uint32_t irer[4];      // 0xE000E180
  volatile uint32_t rsvd7[28];    // 0xE000E190
  volatile uint32_t ipsr[4];      // 0xE000E200
  volatile uint32_t rsvd8[28];    // 0xE000E210
  volatile uint32_t iprr[4];      // 0xE000E280
  volatile uint32_t rsvd9[28];    // 0xE000E290
  volatile uint32_t iactr[4];     // 0xE000E300
  volatile uint32_t rsvd10[60];   // 0xE000E310
  volatile uint32_t iprior[64];   // 0xE000E400
  volatile uint32_t rsvd11[516];  // 0xE000E500
  volatile uint32_t sctlr;        // 0xE000ED10
};
extern struct PFICRegMap pfic;

#if LIBCH32_DEVICE_ID == WCH_CH32V203G6U6
enum PFICIntNum {
  PFIC_DMA1_CH1_INT_NUM = 27,  // Addr: 0x0000006C
  PFIC_DMA1_CH4_INT_NUM = 30,  // Addr: 0x00000078
  PFIC_USART1_INT_NUM = 53,    // Addr: 0x000000D4
};
#else
#erorr "unsupported device"
#endif
/**
 * Prototypes from here down
 */

/**
 * @brief Delay for given number of us
 *
 * @param duration delay time in us
 */
void core_delay_us(uint32_t duration);

/**
 * @brief Delay for given number of ms
 *
 * @param duration duration delay time in ms
 */
void core_delay_ms(uint32_t duration);

/**
 * @brief Enable global interrupts
 *
 */
__attribute__((always_inline)) inline void core_enable_irq() {
  __asm volatile("csrs 0x800, %0" : : "r"(0x88));
}

/**
 * @brief Disable global interrupts
 *
 */
__attribute__((always_inline)) inline void core_disable_irq() {
  __asm volatile("csrc 0x800, %0" : : "r"(0x88));
}

/**
 * @brief Enable selected PFIC interrupt
 *
 */
__attribute__((always_inline)) inline void core_enable_pfic_irq(
    enum PFICIntNum in) {
  pfic.ienr[(uint32_t)in / 32] = 1 << ((uint32_t)in & 0x1F);
}

/**
 * @brief Disable selected PFIC interrupt
 *
 */
__attribute__((always_inline)) inline void core_disable_pfic_irq(
    enum PFICIntNum in) {
  pfic.irer[(uint32_t)in / 32] = 1 << ((uint32_t)in & 0x1F);
}

/**
 * @brief Sets selected PFIC interrupt pending flag
 *
 */
__attribute__((always_inline)) inline void core_set_pending_pfic_irq(
    enum PFICIntNum in) {
  pfic.ipsr[(uint32_t)in / 32] = 1 << ((uint32_t)in & 0x1F);
}

/**
 * @brief Clears selected PFIC interrupt pending flag
 *
 */
__attribute__((always_inline)) inline void core_clear_pending_pfic_irq(
    enum PFICIntNum in) {
  pfic.iprr[(uint32_t)in / 32] = 1 << ((uint32_t)in & 0x1F);
}

/**
 * @brief Activates (triggers) selected PFIC interrupt
 *
 */
__attribute__((always_inline)) inline void core_activate_pfic_irq(
    enum PFICIntNum in) {
  pfic.iactr[(uint32_t)in / 32] = 1 << ((uint32_t)in & 0x1F);
}

/**
 * @brief Checks if selected interrupt is enabled
 *
 * @return uint32_t None zero values means set
 */
__attribute__((always_inline)) inline uint32_t core_check_pfic_irq(
    enum PFICIntNum in) {
  return pfic.isr[(uint32_t)in / 32] &= ~(1 << ((uint32_t)in & 0x1F));
}