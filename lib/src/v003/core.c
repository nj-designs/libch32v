/**
 * @file v003/core.c
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

/**
 * @brief System Tick Register Map
 *
 */
struct SysTickRegMap {
  volatile uint32_t ctrl;
  volatile uint32_t sr;
  volatile uint32_t cnt;
  volatile uint32_t rsvd_0c;
  volatile uint32_t cmp;
  volatile uint32_t rsvd_14;
};

static struct SysTickRegMap __attribute__((section(".systick"))) systick;

struct PFICRegMap __attribute__((section(".pfic"))) pfic;

static uint32_t _us_tick_count;  // systicks per us

void core_delay_us(uint32_t duration) {
  (void)duration;
  if (_us_tick_count == 0) {
    uint32_t hclk_freq = rcc_get_clk_freq(RCC_CLOCK_ID_HCLK);
    _us_tick_count     = hclk_freq / 8000000;  // 8... as STCLK=HCLK/8
  }

  systick.ctrl = 0;
  systick.sr   = 0;
  systick.cnt  = 0;
  systick.cmp  = (uint32_t)duration * (uint32_t)_us_tick_count;
  systick.ctrl = STK_CTLR_STE;
  while (1) {
    if (systick.sr & STK_SR_CNTIF) {
      break;
    }
  }
  systick.ctrl = 0;
}

void core_delay_ms(uint32_t duration) {
  core_delay_us(duration * 1000);
}

void* memset(void* ptr_in, int value, size_t count) {
  uint8_t* ptr = (uint8_t*)ptr_in;
  for (size_t i = 0; i < count; i++) {
    ptr[i] = (uint8_t)value;
  }
  return ptr_in;
}

void core_pfic_set_int_priority(enum PFICIntNum in, enum PFICIntPriority prio) {
#if defined(LIBCH32_V203_FAMILY) || defined(LIBCH32_V307_FAMILY)
#error add me
#elif defined(LIBCH32_V003_FAMILY)
  union {
    struct {
      uint8_t res : 7;  // 6:0
      uint8_t pri : 1;  // 7
    } sub[4];
    uint32_t dword;
  } reg;
#else
#error Port me
#endif
  reg.dword                = pfic.iprior[(int)in / 4];
  reg.sub[in & 0b11].pri   = (uint8_t)prio;
  pfic.iprior[(int)in / 4] = reg.dword;
}

void core_init(void) {
  // Initially set all PFIC interrupts to lower priority
  for (int idx = 0; idx < 256; idx++) {
    core_pfic_set_int_priority(idx, PFIC_INT_PRIORITY_LOWEST);
  }
}

enum PFICIntPriority core_pfic_set_int_priority_threshold(
    enum PFICIntPriority prio) {
  enum PFICIntPriority current_threshold = (pfic.ithresdr >> 7) & 0b1;
  pfic.ithresdr                          = (uint32_t)prio << 7;
  return current_threshold;
}
