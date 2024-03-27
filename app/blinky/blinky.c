/********************************** (C) COPYRIGHT
 ******************************** File Name          : main.c Author : WCH
 * Version            : V1.0.0
 * Date               : 2022/08/08
 * Description        : Main program body.
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * SPDX-License-Identifier: Apache-2.0
 *******************************************************************************/

/*
 *@Note
 GPIO���̣�
 PD0���������

*/

#include "debug.h"

volatile uint32_t fooBar = 0xDEADBEAF;
const char* const test_string = "Hello World";

/*********************************************************************
 * @fn      GPIO_Toggle_INIT
 *
 * @brief   Initializes GPIOA.0
 *
 * @return  none
 */
void GPIO_Toggle_INIT(void) {
  GPIO_InitTypeDef GPIO_InitStructure = {0};

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
}

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void) {
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  Delay_Init();

  GPIO_Toggle_INIT();

  while (1) {
    GPIO_WriteBit(GPIOD, GPIO_Pin_4, Bit_RESET);
    Delay_Ms(5000);
    GPIO_WriteBit(GPIOD, GPIO_Pin_4, Bit_SET);
    Delay_Ms(5000);
  }
}
