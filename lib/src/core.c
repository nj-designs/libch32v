/**
 * @file core.c
 * @author Neil Johnson (nj.designs@protonmail.com)
 * @brief
 * @version 0.1
 * @date 2024-03-19
 *
 * @copyright Copyright (c) 2024
 *
 */
#include <stddef.h>
#include <stdint.h>

#include "core.h"
#include "rcc.h"

struct SysTickRegMap __attribute__((section(".systick"))) systick;

struct PFICRegMap __attribute__((section(".pfic"))) pfic;

static uint32_t _us_tick_count; // systicks per us
static uint32_t _ms_tick_count; // systicks per ms

#if LIBCH32_SYS_TICK_WIDTH == 32
void core_delay_us(uint32_t duration) {
  (void)duration;
  if (_us_tick_count == 0) {
    const struct RCCCfgValues *clk = get_clk_values();
    _us_tick_count = clk->hclk_freq / 8000000; // 8... as STCLK=HCLK/8
    _ms_tick_count = _us_tick_count * 1000;
  }

  systick.sr &= ~(STK_SR_CNTIF); // Clear compare flag
  systick.ctrl = 0;
  systick.cnt = 0;
  // systick.cmp = (uint32_t)duration * (uint32_t)_us_tick_count;
  systick.cmp = (uint32_t)8000000;
  systick.ctrl |= STK_CTLR_STE;
  while (1) {
    if (systick.sr & STK_SR_CNTIF) {
      break;
    }
  }
  systick.ctrl &= ~STK_CTLR_STE;
}
#else
void core_delay_us(uint32_t duration) {
  if (_us_tick_count == 0) {
    _us_tick_count = rcc_get_clk_freq(RCC_CLOCK_ID_HCLK) / 8000000; // 8... as STCLK=HCLK/8
    _ms_tick_count = _us_tick_count * 1000;
  }

  systick.sr &= ~(STK_SR_CNTIF); // Clear compare flag
  systick.ctrl |= STK_CTLR_MODE; // Downcount
  systick.cmp = (uint64_t)duration * (uint64_t)_us_tick_count;
  systick.ctrl |= STK_CTLR_INIT | STK_CTLR_STE;
  while (1) {
    if (systick.sr & STK_SR_CNTIF) {
      break;
    }
  }
  systick.ctrl &= ~STK_CTLR_STE;
}
#endif

void core_delay_ms(uint32_t duration) {
  core_delay_us(duration * 1000);
}

void *memset(void *ptr_in, int value, size_t count) {
  uint8_t *ptr = (uint8_t *)ptr_in;
  for (size_t i = 0; i < count; i++) {
    ptr[i] = (uint8_t)value;
  }
  return ptr_in;
}

void core_pfic_set_int_priority(enum PFICIntNum in, enum PFICIntPriority prio) {
/* #if LIBCH32_INT_NEST_DEPTH == 8
  union {
    struct {
      uint8_t res :5; // 4:0
      uint8_t pri :3; // 7:5
    } sub[4];
    uint32_t dword;
  } reg;
#elif LIBCH32_INT_NEST_DEPTH == 4
  union {
    struct {
      uint8_t res :6; // 5:0
      uint8_t pri :2; // 7:6
    } sub[4];
    uint32_t dword;
  } reg;
#else
  union {
    struct {
      uint8_t res :7; // 6:0
      uint8_t pri :1; // 7
    } sub[4];
    uint32_t dword;
  } reg;
#endif */
#if defined(LIBCH32_CPU_CORE_V4B)
  union {
    struct {
      uint8_t res :5; // 4:0
      uint8_t pri :3; // 7:5
    } sub[4];
    uint32_t dword;
  } reg;
#else
#error unsuported device
#endif
  reg.dword = pfic.iprior[(int)in / 4];
  reg.sub[in & 0b11].pri = (uint8_t)prio;
  pfic.iprior[(int)in / 4] = reg.dword;
}

void core_init(void) {

  // Initially set all PFIC interrupts to lower priority
  for (int idx = 0; idx < 256; idx++) {
    core_pfic_set_int_priority(idx, PFIC_INT_PRIORITY_LOWEST);
  }
}

enum PFICIntPriority core_pfic_set_int_priority_threshold(enum PFICIntPriority prio) {
  enum PFICIntPriority current_threshold = (pfic.ithresdr >> 5) & 0b111;
  pfic.ithresdr = (uint32_t)prio << 5;
  return current_threshold;
}
