/**
 * @file vector.S
 * @author Neil Johnson (nj.designs@protonmail.com)
 * @brief
 * @version 0.1
 * @date 2024-03-24
 *
 * @copyright Copyright (c) 2024
 *
 */

	.section	.init,"ax",@progbits
	.global	_start
	.align	1
_start:
	j	handle_reset

    .section    .vector,"ax",@progbits
    .global _vector_base
    .align  1
/* See Table 9-2 of CH32FV2x_V3xRM.PDF*/
_vector_base:
    .option norvc;                     // We don't want compressed instructions here (Don't think needed!!)
    .word   _start                     // 0x00000000 #0  Not used
    .word   0                          // 0x00000004 #1  Not used
    .word   int_handler_nmi            // 0x00000008 #2  NMI
    .word   int_handler_hard_fault     // 0x0000000C #3  HardFault
    .word   0                          // 0x00000010 #4  Reserved
    .word   int_handler_ecall_m        // 0x00000014 #5  Ecall-M
    .word   0                          // 0x00000018 #6  Reserved
    .word   0                          // 0x0000001C #7  Reserved
    .word   int_handler_ecall_u        // 0x00000020 #8  Ecall-U
    .word   Break_Point_Handler        /* Break Point */
    .word   0
    .word   0
    .word   SysTick_Handler            /* SysTick */
    .word   0
    .word   SW_Handler                 /* SW */
    .word   0
    /* External Interrupts */
    .word   WWDG_IRQHandler            /* Window Watchdog */
    .word   PVD_IRQHandler             /* PVD through EXTI Line detect */
    .word   TAMPER_IRQHandler          /* TAMPER */
    .word   RTC_IRQHandler             /* RTC */
    .word   FLASH_IRQHandler           /* Flash */
    .word   RCC_IRQHandler             /* RCC */
    .word   EXTI0_IRQHandler           /* EXTI Line 0 */
    .word   EXTI1_IRQHandler           /* EXTI Line 1 */
    .word   EXTI2_IRQHandler           /* EXTI Line 2 */
    .word   EXTI3_IRQHandler           /* EXTI Line 3 */
    .word   EXTI4_IRQHandler           /* EXTI Line 4 */
    .word   DMA1_Channel1_IRQHandler   /* DMA1 Channel 1 */
    .word   DMA1_Channel2_IRQHandler   /* DMA1 Channel 2 */
    .word   DMA1_Channel3_IRQHandler   /* DMA1 Channel 3 */
    .word   int_handler_dma1_chan4     // 0x00000078 #30  DMA1_CH4
    .word   DMA1_Channel5_IRQHandler   /* DMA1 Channel 5 */
    .word   DMA1_Channel6_IRQHandler   /* DMA1 Channel 6 */
    .word   DMA1_Channel7_IRQHandler   /* DMA1 Channel 7 */
    .word   ADC1_2_IRQHandler          /* ADC1_2 */
    .word   USB_HP_CAN1_TX_IRQHandler  /* USB HP and CAN1 TX */
    .word   USB_LP_CAN1_RX0_IRQHandler /* USB LP and CAN1RX0 */
    .word   CAN1_RX1_IRQHandler        /* CAN1 RX1 */
    .word   CAN1_SCE_IRQHandler        /* CAN1 SCE */
    .word   EXTI9_5_IRQHandler         /* EXTI Line 9..5 */
    .word   TIM1_BRK_IRQHandler        /* TIM1 Break */
    .word   TIM1_UP_IRQHandler         /* TIM1 Update */
    .word   TIM1_TRG_COM_IRQHandler    /* TIM1 Trigger and Commutation */
    .word   TIM1_CC_IRQHandler         /* TIM1 Capture Compare */
    .word   TIM2_IRQHandler            /* TIM2 */
    .word   TIM3_IRQHandler            /* TIM3 */
    .word   TIM4_IRQHandler            /* TIM4 */
    .word   I2C1_EV_IRQHandler         /* I2C1 Event */
    .word   I2C1_ER_IRQHandler         /* I2C1 Error */
    .word   I2C2_EV_IRQHandler         /* I2C2 Event */
    .word   I2C2_ER_IRQHandler         /* I2C2 Error */
    .word   SPI1_IRQHandler            /* SPI1 */
    .word   SPI2_IRQHandler            /* SPI2 */
    .word   int_handler_usart1         // 0x000000D4 #53 USART1
    .word   USART2_IRQHandler          /* USART2 */
    .word   USART3_IRQHandler          /* USART3 */
    .word   EXTI15_10_IRQHandler       /* EXTI Line 15..10 */
    .word   RTCAlarm_IRQHandler        /* RTC Alarm through EXTI Line */
    .word   USBWakeUp_IRQHandler       /* USB Wake up from suspend */
    .word   TIM8_BRK_IRQHandler        /* TIM8 break interrupt */
    .word   TIM8_UP_IRQHandler        /* TIM8 update interrupt */
    .word   TIM8_TRG_COM_IRQHandler    /*  TIM8 trigger and communication */
    .word   TIM8_CC_IRQHandler        /*  TIM8 capture compare interrupt */
    .word   RNG_IRQHandler        /*  RNG global int*/
    .word   0                           /* Reserved : 0x00000100 */
    .word   SDIO_IRQHandler             /* SDIO global interrupt */
    .word   TIM5_IRQHandler             /* TIM5 global interrupt */
    .word   SPI3_IRQHandler             /* SPI3 global interrupt */
    .word   USART4_IRQHandler             /* USART4 global interrupt */
    .word   USART5_IRQHandler             /* USART5 global interrupt */
    .word   TIM6_IRQHandler             /* TIM6 global interrupt */
    .word   TIM7_IRQHandler             /* TIM7 global interrupt */
    .word   DMA2_CH1_IRQHandler             /* DMA2 channel1 global interrupt */
    .word   DMA2_CH2_IRQHandler             /* DMA2 channel2 global interrupt */
    .word   DMA2_CH3_IRQHandler             /* DMA2 channel3 global interrupt */
    .word   DMA2_CH4_IRQHandler             /* DMA2 channel4 global interrupt */
    .word   DMA2_CH5_IRQHandler             /* DMA2 channel5 global interrupt */
    .word   ETH_IRQHandler                  /* ETH global interrupt */
    .word   ETH_WKUP_IRQHandler                  /* ETH_WKUP global interrupt */
    .word   CAN2_TX_IRQHandler                  /* CAN2_TX global interrupt */
    .word   CAN2_RX0_IRQHandler                  /* CAN2_TX global interrupt */
    .word   CAN2_RX1_IRQHandler                  /* CAN2_TX global interrupt */
    .word   CAN2_SCE_IRQHandler                  /* CAN2_TX global interrupt */
    .word   OTG_FS_IRQHandler                  /* OTG_FS global interrupt */
    .word   USBHS_WAKE_UP_IRQHandler           /* USBHS global interrupt */
    .word   USBHS_IRQHandler           /* USBHS global interrupt */
    .word   DVP_IRQHandler           /* DVP global interrupt */
    .word   USART6_IRQHandler           /* USART6 global interrupt */
    .word   USART7_IRQHandler           /* USART8 global interrupt */
    .word   USART8_IRQHandler           /* USART8 global interrupt */
    .word   TIM9_BRK_IRQHandler
    .word   TIM9_UP_IRQHandler
    .word   TIM9_TRG_COM_IRQHandler
    .word   TIM9_CC_IRQHandler
    .word   TIM10_BRK_IRQHandler
    .word   TIM10_UP_IRQHandler
    .word   TIM10_TRG_COM_IRQHandler
    .word   TIM10_CC_IRQHandler
    .word   DMA2_CH6_IRQHandler
    .word   DMA2_CH7_IRQHandler
    .word   DMA2_CH8_IRQHandler
    .word   DMA2_CH9_IRQHandler
    .word   DMA2_CH10_IRQHandler
    .word   DMA2_CH11_IRQHandler


    .option rvc;                        // Enable ompressed instructions
    .section    .text.vector_handler, "ax", @progbits
    .weak   int_handler_nmi               // 0x00000008 #2  NMI
    .weak   int_handler_hard_fault          /* Hard Fault */
    .weak   int_handler_ecall_m       /* Ecall M Mode */
    .weak   int_handler_ecall_u       /* Ecall U Mode */
    .weak   Break_Point_Handler        /* Break Point */
    .weak   SysTick_Handler            /* SysTick */
    .weak   SW_Handler                 /* SW */
    .weak   WWDG_IRQHandler            /* Window Watchdog */
    .weak   PVD_IRQHandler             /* PVD through EXTI Line detect */
    .weak   TAMPER_IRQHandler          /* TAMPER */
    .weak   RTC_IRQHandler             /* RTC */
    .weak   FLASH_IRQHandler           /* Flash */
    .weak   RCC_IRQHandler             /* RCC */
    .weak   EXTI0_IRQHandler           /* EXTI Line 0 */
    .weak   EXTI1_IRQHandler           /* EXTI Line 1 */
    .weak   EXTI2_IRQHandler           /* EXTI Line 2 */
    .weak   EXTI3_IRQHandler           /* EXTI Line 3 */
    .weak   EXTI4_IRQHandler           /* EXTI Line 4 */
    .weak   DMA1_Channel1_IRQHandler   /* DMA1 Channel 1 */
    .weak   DMA1_Channel2_IRQHandler   /* DMA1 Channel 2 */
    .weak   DMA1_Channel3_IRQHandler   /* DMA1 Channel 3 */
    .weak   int_handler_dma1_chan4   /* DMA1 Channel 4 */
    .weak   DMA1_Channel5_IRQHandler   /* DMA1 Channel 5 */
    .weak   DMA1_Channel6_IRQHandler   /* DMA1 Channel 6 */
    .weak   DMA1_Channel7_IRQHandler   /* DMA1 Channel 7 */
    .weak   ADC1_2_IRQHandler          /* ADC1_2 */
    .weak   USB_HP_CAN1_TX_IRQHandler  /* USB HP and CAN1 TX */
    .weak   USB_LP_CAN1_RX0_IRQHandler /* USB LP and CAN1RX0 */
    .weak   CAN1_RX1_IRQHandler        /* CAN1 RX1 */
    .weak   CAN1_SCE_IRQHandler        /* CAN1 SCE */
    .weak   EXTI9_5_IRQHandler         /* EXTI Line 9..5 */
    .weak   TIM1_BRK_IRQHandler        /* TIM1 Break */
    .weak   TIM1_UP_IRQHandler         /* TIM1 Update */
    .weak   TIM1_TRG_COM_IRQHandler    /* TIM1 Trigger and Commutation */
    .weak   TIM1_CC_IRQHandler         /* TIM1 Capture Compare */
    .weak   TIM2_IRQHandler            /* TIM2 */
    .weak   TIM3_IRQHandler            /* TIM3 */
    .weak   TIM4_IRQHandler            /* TIM4 */
    .weak   I2C1_EV_IRQHandler         /* I2C1 Event */
    .weak   I2C1_ER_IRQHandler         /* I2C1 Error */
    .weak   I2C2_EV_IRQHandler         /* I2C2 Event */
    .weak   I2C2_ER_IRQHandler         /* I2C2 Error */
    .weak   SPI1_IRQHandler            /* SPI1 */
    .weak   SPI2_IRQHandler            /* SPI2 */
    .weak   int_handler_usart1          /* USART1 */
    .weak   USART2_IRQHandler          /* USART2 */
    .weak   USART3_IRQHandler          /* USART3 */
    .weak   EXTI15_10_IRQHandler       /* EXTI Line 15..10 */
    .weak   RTCAlarm_IRQHandler        /* RTC Alarm through EXTI Line */
    .weak   USBWakeUp_IRQHandler       /* USB Wakeup from suspend */
    .weak   USBFS_IRQHandler           /* USBFS */
    .weak   USBFSWakeUp_IRQHandler     /* USBFS Wake Up */
    .weak   UART4_IRQHandler           /* UART4 */
    .weak   DMA1_Channel8_IRQHandler   /* DMA1 Channel8 */
    .weak   TIM8_BRK_IRQHandler        /* TIM8 break interrupt */
    .weak   TIM8_UP_IRQHandler        /* TIM8 update interrupt */
    .weak   TIM8_TRG_COM_IRQHandler    /*  TIM8 trigger and communication */
    .weak   TIM8_CC_IRQHandler        /*  TIM8 capture compare interrupt */
    .weak   RNG_IRQHandler        /*  RNG global int*/
    .weak   SDIO_IRQHandler             /* SDIO global interrupt */
    .weak   TIM5_IRQHandler             /* TIM5 global interrupt */
    .weak   SPI3_IRQHandler             /* SPI3 global interrupt */
    .weak   USART4_IRQHandler             /* USART4 global interrupt */
    .weak   USART5_IRQHandler             /* USART5 global interrupt */
    .weak   TIM6_IRQHandler             /* TIM6 global interrupt */
    .weak   TIM7_IRQHandler             /* TIM7 global interrupt */
    .weak   DMA2_CH1_IRQHandler             /* DMA2 channel1 global interrupt */
    .weak   DMA2_CH2_IRQHandler             /* DMA2 channel2 global interrupt */
    .weak   DMA2_CH3_IRQHandler             /* DMA2 channel3 global interrupt */
    .weak   DMA2_CH4_IRQHandler             /* DMA2 channel4 global interrupt */
    .weak   DMA2_CH5_IRQHandler             /* DMA2 channel5 global interrupt */
    .weak   ETH_IRQHandler             /* ETH global interrupt */
    .weak   ETH_WKUP_IRQHandler                  /* ETH_WKUP global interrupt */
    .weak   CAN2_TX_IRQHandler                  /* CAN2_TX global interrupt */
    .weak   CAN2_RX0_IRQHandler                  /* CAN2_TX global interrupt */
    .weak   CAN2_RX1_IRQHandler                  /* CAN2_TX global interrupt */
    .weak   CAN2_SCE_IRQHandler                  /* CAN2_TX global interrupt */
    .weak   OTG_FS_IRQHandler                  /* OTG_FS global interrupt */
    .weak   USBHS_WAKE_UP_IRQHandler           /* USBHS global interrupt */
    .weak   USBHS_IRQHandler           /* USBHS global interrupt */
    .weak   DVP_IRQHandler           /* DVP global interrupt */
    .weak   USART6_IRQHandler           /* USART6 global interrupt */
    .weak   USART7_IRQHandler           /* USART8 global interrupt */
    .weak   USART8_IRQHandler           /* USART8 global interrupt */
    .weak   TIM9_BRK_IRQHandler
    .weak   TIM9_UP_IRQHandler
    .weak   TIM9_TRG_COM_IRQHandler
    .weak   TIM9_CC_IRQHandler
    .weak   TIM10_BRK_IRQHandler
    .weak   TIM10_UP_IRQHandler
    .weak   TIM10_TRG_COM_IRQHandler
    .weak   TIM10_CC_IRQHandler
    .weak   DMA2_CH6_IRQHandler
    .weak   DMA2_CH7_IRQHandler
    .weak   DMA2_CH8_IRQHandler
    .weak   DMA2_CH9_IRQHandler
    .weak   DMA2_CH10_IRQHandler
    .weak   DMA2_CH11_IRQHandler

NMI_Handler:
int_handler_hard_fault:
int_handler_ecall_m:
int_handler_ecall_u:
Break_Point_Handler:
SysTick_Handler:
SW_Handler:
WWDG_IRQHandler:
PVD_IRQHandler:
TAMPER_IRQHandler:
RTC_IRQHandler:
FLASH_IRQHandler:
RCC_IRQHandler:
EXTI0_IRQHandler:
EXTI1_IRQHandler:
EXTI2_IRQHandler:
EXTI3_IRQHandler:
EXTI4_IRQHandler:
DMA1_Channel1_IRQHandler:
DMA1_Channel2_IRQHandler:
DMA1_Channel3_IRQHandler:
int_handler_dma1_chan4:
DMA1_Channel5_IRQHandler:
DMA1_Channel6_IRQHandler:
DMA1_Channel7_IRQHandler:
ADC1_2_IRQHandler:
USB_HP_CAN1_TX_IRQHandler:
USB_LP_CAN1_RX0_IRQHandler:
CAN1_RX1_IRQHandler:
CAN1_SCE_IRQHandler:
EXTI9_5_IRQHandler:
TIM1_BRK_IRQHandler:
TIM1_UP_IRQHandler:
TIM1_TRG_COM_IRQHandler:
TIM1_CC_IRQHandler:
TIM2_IRQHandler:
TIM3_IRQHandler:
TIM4_IRQHandler:
I2C1_EV_IRQHandler:
I2C1_ER_IRQHandler:
I2C2_EV_IRQHandler:
I2C2_ER_IRQHandler:
SPI1_IRQHandler:
SPI2_IRQHandler:
int_handler_usart1:
USART2_IRQHandler:
USART3_IRQHandler:
EXTI15_10_IRQHandler:
RTCAlarm_IRQHandler:
USBWakeUp_IRQHandler:
USBFS_IRQHandler:
USBFSWakeUp_IRQHandler:
UART4_IRQHandler:
DMA1_Channel8_IRQHandler:
TIM8_BRK_IRQHandler:
TIM8_UP_IRQHandler:
TIM8_TRG_COM_IRQHandler:
TIM8_CC_IRQHandler:
RNG_IRQHandler:
SDIO_IRQHandler:
TIM5_IRQHandler:
SPI3_IRQHandler:
USART4_IRQHandler:
USART5_IRQHandler:
TIM6_IRQHandler:
TIM6_IRQHandler:
DMA3_CH1_IRQHandler:
DMA3_CH2_IRQHandler:
DMA3_CH3_IRQHandler:
DMA3_CH4_IRQHandler:
DMA3_CH5_IRQHandler:
ETH_IRQHandler:
ETH_WKUP_IRQHandler:
CAN2_TX_IRQHandler:
CAN2_RX0_IRQHandler:
CAN2_RX1_IRQHandler:
CAN2_SCE_IRQHandler:
OTG_FS_IRQHandler:
USBHS_WAKE_UP_IRQHandler:
USBHS_IRQHandler:
DVP_IRQHandler:
USART6_IRQHandler:
USART7_IRQHandler:
USART8_IRQHandler:
TIM9_BRK_IRQHandler:
TIM9_UP_IRQHandler:
TIM10_UP_IRQHandler:
TIM10_TRG_COM_IRQHandler:
TIM10_CC_IRQHandler:
TIM11_BRK_IRQHandler:
TIM11_UP_IRQHandler:
TIM11_TRG_COM_IRQHandler:
TIM11_CC_IRQHandler:
DMA3_CH6_IRQHandler:
DMA3_CH7_IRQHandler:
DMA3_CH8_IRQHandler:
DMA3_CH9_IRQHandler:
DMA3_CH10_IRQHandler:
DMA3_CH11_IRQHandler:
1:
	j 1b
