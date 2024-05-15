/**
 * @file maths.c
 * @author Neil Johnson (nj.designs@protonmail.com)
 * @brief  Very slow versions of integer arithmetic functions.
 * @version 0.1
 * @date 2024-05-14
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "device_config.h"

#if LIBCH32_DEVICE_ID == WCH_CH32V003F4

/**
 * @brief Return the result of a / b
 *
 * @param a
 * @param b
 * @return unsigned
 */
unsigned int __udivsi3(unsigned int a, unsigned int b);
unsigned int __udivsi3(unsigned int a, unsigned int b) {
  unsigned int res = 0;

  if (b) {
    while (a >= b) {
      a -= b;
      res++;
    }
  }

  return res;
}

/**
 * @brief
 *
 * @param a
 * @param b
 * @return unsigned
 */
unsigned int __umodsi3(unsigned int a, unsigned int b);
unsigned int __umodsi3(unsigned int a, unsigned int b) {
  unsigned int r = 0, oa = a;

  if (b) {
    while (a >= b) {
      a -= b;
      r += b;
    }
  }

  return oa - r;
}

/**
 * @brief
 *
 * @param a
 * @param b
 * @return long
 */
long __muldi3(long a, long b);
long __muldi3(long a, long b) {
  long i;
  long res = 0;

  if (a > b) {
    for (i = 0; i < b; i++) {
      res += a;
    }
  } else {
    for (i = 0; i < a; i++) {
      res += b;
    }
  }
  return res;
}

/**
 * @brief
 *
 * @param a
 * @param b
 * @return int
 */
int __mulsi3(int a, int b);
int __mulsi3(int a, int b) {
  int i, res = 0;

  if (a > b) {
    for (i = 0; i < b; i++) {
      res += a;
    }
  } else {
    for (i = 0; i < a; i++) {
      res += b;
    }
  }
  return res;
}

#endif