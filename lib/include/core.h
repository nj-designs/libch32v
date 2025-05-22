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

#define NJD_IRQ_ATTRIBUTE __attribute__((interrupt))

/**
 * @brief System Tick Register Map
 *
 */
struct SysTickRegMap {
  volatile uint32_t ctrl;
  volatile uint32_t sr;
#if LIBCH32_SYS_TICK_WIDTH == 32
  volatile uint32_t cnt;
  volatile uint32_t rsvd_0c;
  volatile uint32_t cmp;
  volatile uint32_t rsvd_14;
#else
  volatile uint64_t cnt;
  volatile uint64_t cmp;
#endif
};

extern struct SysTickRegMap systick;

// STK_CTLR
//---------
// SWIE[31]
static const uint32_t STK_CTLR_SWIE = (1 << 31);
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
  volatile uint32_t isr[4];      // 0xE000E000
  volatile uint32_t rsvd1[4];    // 0xE000E010
  volatile uint32_t ipr[4];      // 0xE000E020
  volatile uint32_t rsvd2[4];    // 0xE000E030
  volatile uint32_t ithresdr;    // 0xE000E040
  volatile uint32_t rsvd3;       // 0xE000E044
  volatile uint32_t cfgr;        // 0xE000E048
  volatile uint32_t gisr;        // 0xE000E04C
  volatile uint32_t vtfidr;      // 0xE000E050
  volatile uint32_t rsvd4[3];    // 0xE000E054
  volatile uint32_t vtfaddrr[4]; // 0xE000E060
  volatile uint32_t rsvd5[36];   // 0xE000E070
  volatile uint32_t ienr[4];     // 0xE000E100
  volatile uint32_t rsvd6[28];   // 0xE000E110
  volatile uint32_t irer[4];     // 0xE000E180
  volatile uint32_t rsvd7[28];   // 0xE000E190
  volatile uint32_t ipsr[4];     // 0xE000E200
  volatile uint32_t rsvd8[28];   // 0xE000E210
  volatile uint32_t iprr[4];     // 0xE000E280
  volatile uint32_t rsvd9[28];   // 0xE000E290
  volatile uint32_t iactr[4];    // 0xE000E300
  volatile uint32_t rsvd10[60];  // 0xE000E310
  volatile uint32_t iprior[64];  // 0xE000E400
  volatile uint32_t rsvd11[516]; // 0xE000E500
  volatile uint32_t sctlr;       // 0xE000ED10
};
extern struct PFICRegMap pfic;

#if 0
// PFIC interrupt priority levels
#if LIBCH32_INT_NEST_DEPTH == 8
enum PFICIntPriority : uint8_t {
  PFIC_INT_PRIORITY_0 = 0b111,
  PFIC_INT_PRIORITY_1 = 0b110,
  PFIC_INT_PRIORITY_2 = 0b101,
  PFIC_INT_PRIORITY_3 = 0b100,
  PFIC_INT_PRIORITY_4 = 0b011,
  PFIC_INT_PRIORITY_5 = 0b010,
  PFIC_INT_PRIORITY_6 = 0b001,
  PFIC_INT_PRIORITY_7 = 0b000,

  PFIC_INT_PRIORITY_LOWEST = PFIC_INT_PRIORITY_0,
  PFIC_INT_PRIORITY_HIGHEST = PFIC_INT_PRIORITY_7,
};
#elif LIBCH32_INT_NEST_DEPTH == 4
enum PFICIntPriority : uint8_t {
  PFIC_INT_PRIORITY_0 = 0b11,
  PFIC_INT_PRIORITY_1 = 0b10,
  PFIC_INT_PRIORITY_2 = 0b01,
  PFIC_INT_PRIORITY_3 = 0b00,

  PFIC_INT_PRIORITY_LOWEST = PFIC_INT_PRIORITY_0,
  PFIC_INT_PRIORITY_HIGHEST = PFIC_INT_PRIORITY_3,
};
#elif LIBCH32_INT_NEST_DEPTH == 2
enum PFICIntPriority : uint8_t {
  PFIC_INT_PRIORITY_0 = 0b1,
  PFIC_INT_PRIORITY_1 = 0b0,

  PFIC_INT_PRIORITY_LOWEST = PFIC_INT_PRIORITY_0,
  PFIC_INT_PRIORITY_HIGHEST = PFIC_INT_PRIORITY_1,
};
#endif

#endif

#if defined(LIBCH32_CPU_CORE_V4B)
enum PFICIntPriority : uint8_t {
  PFIC_INT_PRIORITY_0 = 0b111,
  PFIC_INT_PRIORITY_1 = 0b110,
  PFIC_INT_PRIORITY_2 = 0b101,
  PFIC_INT_PRIORITY_3 = 0b100,
  PFIC_INT_PRIORITY_4 = 0b011,
  PFIC_INT_PRIORITY_5 = 0b010,
  PFIC_INT_PRIORITY_6 = 0b001,
  PFIC_INT_PRIORITY_7 = 0b000,

  PFIC_INT_PRIORITY_LOWEST = PFIC_INT_PRIORITY_0,
  PFIC_INT_PRIORITY_HIGHEST = PFIC_INT_PRIORITY_7,
};
#else
#error "unsupported device"
#endif

#if defined(LIBCH32_V203_FAMILY) || defined(LIBCH32_V307_FAMILY)
enum PFICIntNum : uint8_t {
  PFIC_DMA1_CH1_INT_NUM = 27, // Addr: 0x0000006C
  PFIC_DMA1_CH4_INT_NUM = 30, // Addr: 0x00000078
  PFIC_CAN1_RX0_INT_NUM = 36,
  PFIC_CAN1_RX1_INT_NUM = 37,
  PFIC_USART1_INT_NUM = 53, // Addr: 0x000000D4
};
#elif defined(LIBCH32_V003_FAMILY)
enum PFICIntNum {
  PFIC_DMA1_CH4_INT_NUM = 25, // Addr: 0x00000064
};
#else
#error "unsupported device"
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
__attribute__((always_inline)) inline void
core_enable_pfic_irq(enum PFICIntNum in) {
  pfic.ienr[(uint32_t)in / 32] = 1 << ((uint32_t)in & 0x1F);
}

/**
 * @brief Disable selected PFIC interrupt
 *
 */
__attribute__((always_inline)) inline void
core_disable_pfic_irq(enum PFICIntNum in) {
  pfic.irer[(uint32_t)in / 32] = 1 << ((uint32_t)in & 0x1F);
}

/**
 * @brief Sets selected PFIC interrupt pending flag
 *
 */
__attribute__((always_inline)) inline void
core_set_pending_pfic_irq(enum PFICIntNum in) {
  pfic.ipsr[(uint32_t)in / 32] = 1 << ((uint32_t)in & 0x1F);
}

/**
 * @brief Clears selected PFIC interrupt pending flag
 *
 */
__attribute__((always_inline)) inline void
core_clear_pending_pfic_irq(enum PFICIntNum in) {
  pfic.iprr[(uint32_t)in / 32] = 1 << ((uint32_t)in & 0x1F);
}

/**
 * @brief Activates (triggers) selected PFIC interrupt
 *
 */
__attribute__((always_inline)) inline void
core_activate_pfic_irq(enum PFICIntNum in) {
  pfic.iactr[(uint32_t)in / 32] = 1 << ((uint32_t)in & 0x1F);
}

/**
 * @brief Checks if selected interrupt is enabled
 *
 * @return uint32_t None zero values means set
 */
__attribute__((always_inline)) inline uint32_t
core_check_pfic_irq(enum PFICIntNum in) {
  return pfic.isr[(uint32_t)in / 32] &= ~(1 << ((uint32_t)in & 0x1F));
}

/**
 * @brief Init PFIC and other core HW
 *
 */
void core_init(void);

/**
 * @brief Set priority of specified interrupt
 */
void core_pfic_set_int_priority(enum PFICIntNum in, enum PFICIntPriority prio);

enum PFICIntPriority
core_pfic_set_int_priority_threshold(enum PFICIntPriority prio);
