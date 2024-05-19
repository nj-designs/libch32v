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

#include "rcc.h"
#include "core.h"

struct SysTickRegMap __attribute__((section(".systick"))) systick;

struct PFICRegMap __attribute__((section(".pfic"))) pfic;

static uint32_t _us_tick_count;  // systicks per us
static uint32_t _ms_tick_count;  // systicks per ms

void core_delay_us(uint32_t duration) {
  if (_us_tick_count == 0) {
    const struct RCCCfgValues* clk = get_clk_values();
    _us_tick_count = clk->hclk_freq / 8000000;  // 8... as STCLK=HCLK/8
    _ms_tick_count = _us_tick_count * 1000;
  }

  systick.sr &= ~(STK_SR_CNTIF);  // Clear compare flag
  systick.ctrl |= STK_CTLR_MODE;  // Downcount
  systick.cmp = (uint64_t)duration * (uint64_t)_us_tick_count;
  systick.ctrl |= STK_CTLR_INIT | STK_CTLR_STE;
  while (1) {
    if (systick.sr & STK_SR_CNTIF) {
      break;
    }
  }
  systick.ctrl &= ~STK_CTLR_STE;
}

void core_delay_ms(uint32_t duration) { core_delay_us(duration * 1000); }

void* memset(void* ptr_in, int value, size_t count) {
  uint8_t* ptr = (uint8_t*)ptr_in;
  for (size_t i = 0; i < count; i++) {
    ptr[i] = (uint8_t)value;
  }
  return ptr_in;
}