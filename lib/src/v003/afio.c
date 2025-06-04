#include "afio.h"
#include "rcc.h"

struct AFIORegMap {
  uint32_t res0;
  volatile union {
    struct {
      uint32_t spi1_rm         : 1;  // 0
      uint32_t i2c1_rm         : 1;  // 1
      uint32_t usart1_rm       : 1;  // 2
      uint32_t res5_3          : 3;  // 5:3
      uint32_t tim1_rm         : 2;  // 7:6
      uint32_t tim2_rm         : 2;  // 9:8
      uint32_t res14_10        : 5;  // 14:10
      uint32_t pa12            : 1;  // 15
      uint32_t res16           : 1;  // 16
      uint32_t adc1_etrginj_rm : 1;  // 17
      uint32_t adc1_etrgreg_rm : 1;  // 18
      uint32_t res20_19        : 2;  // 20:19
      uint32_t usart1_rm1      : 1;  // 21
      uint32_t i2c1remap1      : 1;  // 22
      uint32_t tim1_iremap     : 1;  // 23
      uint32_t sw_cfg          : 3;  // 26:24
    };
    uint32_t dword;
  } pcfr1;
  volatile uint32_t exticr;
};

static struct AFIORegMap __attribute__((section(".afio"))) afio;

void afio_remap(enum RCCPeripheralId id, uint32_t rmv) {
  switch (id) {
    case RCC_SPI1_ID: {
      afio.pcfr1.spi1_rm = rmv;
      break;
    }
    default: {
      while (true) {
      }
    }
  }
}
