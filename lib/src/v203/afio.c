#include "afio.h"

struct AFIORegMap {
  volatile uint32_t ecr;
  volatile union {
    struct {
      uint32_t spi1_rm         : 1;  // 0
      uint32_t i2c1_rm         : 1;  // 1
      uint32_t usart1_rm       : 1;  // 2
      uint32_t usart2_rm       : 1;  // 3
      uint32_t usart3_rm       : 2;  // 5:4
      uint32_t tim1_rm         : 2;  // 7:6
      uint32_t tim2_rm         : 2;  // 9:8
      uint32_t tim3_rm         : 2;  // 11:10
      uint32_t tim4_rm         : 1;  // 12
      uint32_t can1_rm         : 2;  // 14:13
      uint32_t pd01_rm         : 1;  // 15
      uint32_t tim5ch4_rm      : 1;  // 16
      uint32_t adc1_etrginj_rm : 1;  // 17
      uint32_t adc1_etrgreg_rm : 1;  // 18
      uint32_t adc2_etrginj_rm : 1;  // 19
      uint32_t adc2_etrgreg_rm : 1;  // 20
      uint32_t eth_rm          : 1;  // 21
      uint32_t can2_rm         : 1;  // 22
      uint32_t mii_rmii_sel    : 1;  // 23
      uint32_t sw_cfg          : 3;  // 26:24
      uint32_t res_27          : 1;  // 27
      uint32_t spi3_rm         : 1;  // 28
      uint32_t tim2itr1_rm     : 1;  // 29
      uint32_t ptp_pps_rm      : 1;  // 30
      uint32_t res_31          : 1;  // 31
    };
    uint32_t dword;
  } pcfr1_;
  volatile uint32_t exticr;
};

static struct AFIORegMap __attribute__((section(".afio"))) afio;

void afio_remap(enum RCCPeripheralId id, uint32_t rmv) {
  switch (id) {
    case RCCCan1Id: {
      afio.pcfr1_.can1_rm = rmv;
      break;
    }
    default: {
      while (true) {
      }
    }
  }
}
