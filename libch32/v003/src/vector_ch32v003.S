  .section  .vector.table, "ax", @progbits
  .globl  _vector_table
  .align 2
_vector_table:
  .option   norvc;
  j reset_handler                   // No.  0 : Reset Handler
  .word   0                         // No.  1 : Reserved
  .word   NMI_Handler               // No.  2 : NMI Handler
  .word   HardFault_Handler         // No.  3 : Hard Fault Handler
  .word   0                         // No.  4 : Reserved
  .word   0                         // No.  5 : Reserved
  .word   0                         // No.  6 : Reserved
  .word   0                         // No.  7 : Reserved
  .word   0                         // No.  8 : Reserved
  .word   0                         // No.  9 : Reserved
  .word   0                         // No. 10 : Reserved
  .word   0                         // No. 11 : Reserved
  .word   SysTick_Handler           // No. 12 : SysTick Handler
  .word   0                         // No. 13   Reserved
  .word   SW_Handler                // No. 14 : SW Handler
  .word   0                         // No. 15 : Reserved
  .word   WWDG_IRQHandler           // No. 16 : Window Watchdog
  .word   PVD_IRQHandler            // No. 17 : PVD through EXTI Line detect
  .word   FLASH_IRQHandler          // No. 18 : Flash
  .word   RCC_IRQHandler            // No. 19 : RCC
  .word   EXTI7_0_IRQHandler        // No. 20 : EXTI Line 7..0
  .word   AWU_IRQHandler            // No. 21 : AWU
  .word   DMA1_Channel1_IRQHandler  // No. 22 : DMA1 Channel 1
  .word   DMA1_Channel2_IRQHandler  // No. 23 : DMA1 Channel 2
  .word   DMA1_Channel3_IRQHandler  // No. 24 : DMA1 Channel 3
  .word   DMA1_Channel4_IRQHandler  // No. 25 : DMA1 Channel 4
  .word   DMA1_Channel5_IRQHandler  // No. 26 : DMA1 Channel 5
  .word   DMA1_Channel6_IRQHandler  // No. 27 : DMA1 Channel 6
  .word   DMA1_Channel7_IRQHandler  // No. 28 : DMA1 Channel 7
  .word   ADC1_IRQHandler          	// No. 29 : ADC1
  .word   I2C1_EV_IRQHandler        // No. 30 : I2C1 Event
  .word   I2C1_ER_IRQHandler        // No. 31 : I2C1 Error
  .word   USART1_IRQHandler         // No. 32 : USART1
	.word   SPI1_IRQHandler           // No. 33 : SPI1
	.word   TIM1_BRK_IRQHandler       // No. 34 : TIM1 Break
  .word   TIM1_UP_IRQHandler        // No. 35 : TIM1 Update
  .word   TIM1_TRG_COM_IRQHandler   // No. 36 : TIM1 Trigger and Commutation
  .word   TIM1_CC_IRQHandler        // No. 37 : TIM1 Capture Compare
  .word   TIM2_IRQHandler           // No. 38 : TIM2

  .option rvc;
  .section  .text.vector_handler, "ax", @progbits
	.weak   NMI_Handler
  NMI_Handler:              1: j 1b
	.weak   HardFault_Handler
  HardFault_Handler:        1: j 1b
	.weak   SysTick_Handler
  SysTick_Handler:          1: j 1b
	.weak   SW_Handler
  SW_Handler:               1: j 1b
	.weak   WWDG_IRQHandler
  WWDG_IRQHandler:          1: j 1b
	.weak   PVD_IRQHandler
  PVD_IRQHandler:           1: j 1b
	.weak   FLASH_IRQHandler
  FLASH_IRQHandler:         1: j 1b
	.weak   RCC_IRQHandler
  RCC_IRQHandler:           1: j 1b
	.weak   EXTI7_0_IRQHandler
  EXTI7_0_IRQHandler:       1: j 1b
	.weak   AWU_IRQHandler
  AWU_IRQHandler:           1: j 1b
	.weak   DMA1_Channel1_IRQHandler
  DMA1_Channel1_IRQHandler: 1: j 1b
	.weak   DMA1_Channel2_IRQHandler
  DMA1_Channel2_IRQHandler: 1: j 1b
	.weak   DMA1_Channel3_IRQHandler
  DMA1_Channel3_IRQHandler: 1: j 1b
	.weak   DMA1_Channel4_IRQHandler
  DMA1_Channel4_IRQHandler: 1: j 1b
	.weak   DMA1_Channel5_IRQHandler
  DMA1_Channel5_IRQHandler: 1: j 1b
	.weak   DMA1_Channel6_IRQHandler
  DMA1_Channel6_IRQHandler: 1: j 1b
	.weak   DMA1_Channel7_IRQHandler
  DMA1_Channel7_IRQHandler: 1: j 1b
	.weak   ADC1_IRQHandler
  ADC1_IRQHandler:          1: j 1b
	.weak   I2C1_EV_IRQHandler
  I2C1_EV_IRQHandler:       1: j 1b
	.weak   I2C1_ER_IRQHandler
  I2C1_ER_IRQHandler:       1: j 1b
	.weak   USART1_IRQHandler
  USART1_IRQHandler:        1: j 1b
	.weak   SPI1_IRQHandler
  SPI1_IRQHandler:          1: j 1b
	.weak   TIM1_BRK_IRQHandler
  TIM1_BRK_IRQHandler:      1: j 1b
	.weak   TIM1_UP_IRQHandler
  TIM1_UP_IRQHandler:       1: j 1b
	.weak   TIM1_TRG_COM_IRQHandler
  TIM1_TRG_COM_IRQHandler:  1: j 1b
	.weak   TIM1_CC_IRQHandler
  TIM1_CC_IRQHandler:       1: j 1b
	.weak   TIM2_IRQHandler
  TIM2_IRQHandler:          1: j 1b
