#include <stdint.h>

volatile uint32_t last_dividend;
volatile uint32_t last_divisor;
volatile uint32_t last_result;
volatile char* last_op;

uint32_t __umodsi3(uint32_t dividend, uint32_t divisor) {
  last_op       = "__umodsi3";
  last_dividend = dividend;
  last_divisor  = divisor;
  while (dividend >= divisor) {
    volatile uint32_t tmp      = divisor;
    volatile uint32_t quotient = 1;
    while (tmp <= dividend) {
      tmp <<= 1;
      quotient <<= 1;
    }
    dividend -= tmp >> 1;
  }
  last_result = dividend;
  return dividend;
}

uint32_t __udivsi3(uint32_t dividend, uint32_t divisor) {
  last_op       = "__udivsi3";
  last_dividend = dividend;
  last_divisor  = divisor;
  uint32_t ans  = 0;

  while (dividend >= divisor) {
    uint32_t tmp      = divisor;
    uint32_t quotient = 1;
    while (tmp <= dividend) {
      tmp <<= 1;
      quotient <<= 1;
    }
    ans += quotient >> 1;
    dividend -= tmp >> 1;
  }
  last_result = ans;
  return ans;
}

int32_t __mulsi3(int32_t a, int32_t b) {
  last_op        = "__mulsi3";
  int32_t result = 0;
  int negative   = 0;

  // Handle sign and convert to positive
  if (a < 0) {
    a        = -a;
    negative = !negative;
  }
  if (b < 0) {
    b        = -b;
    negative = !negative;
  }

  // Shift-and-add multiplication
  while (b) {
    if (b & 1) {
      result += a;
    }
    a <<= 1;
    b >>= 1;
  }

  // Apply sign
  return negative ? -result : result;
}
