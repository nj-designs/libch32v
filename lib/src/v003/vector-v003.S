/**
 * @file vector-v003.S
 * @author Neil Johnson (nj.designs@protonmail.com)
 * @brief
 * @version 0.1
 * @date 2024-03-24
 *
 * @copyright Copyright (c) 2024
 *
 */

    .section    .init,"ax",@progbits
    .global _vector_base
    .global _start
    .align  1
_start:
_vector_base:
    .option norvc;                   // We don't want compressed instructions here (Don't think needed!!)
	  j	handle_reset
    .word   0x00000004               // 0x00000004 #1  Not used
    .word   int_handler_nmi          // 0x00000008 #2  NMI
    .word   int_handler_hard_fault   // 0x0000000C #3  HardFault
    .word   0x00000010               // 0x00000010 #4  
    .word   0x00000014               // 0x00000014 #5 
    .word   0x00000018               // 0x00000018 #6 
    .word   0x0000001C               // 0x0000001C #7 
    .word   0x00000020               // 0x00000020 #8 
    .word   0x00000024               // 0x00000024 #9 
    .word   0x00000028               // 0x00000028 #10
    .word   0x0000002C               // 0x0000002C #11
    .word   int_handler_sys_tick     // 0x00000030 #12 System timer
    .word   0x00000034               // 0x00000034 #13 
    .word   int_handler_sw           // 0x00000038 #14 Sofware
    .word   0x0000003C               // 0x0000003C #15
    /* External Interrupts */
    .word   int_handler_wwdg         // 0x00000040 #16 Window timer
    .word   int_handler_pvd          // 0x00000044 #17 Supply voltage dectection (EXTI)
    .word   int_handler_flash        // 0x00000048 #18 Flash global
    .word   int_handler_rcc          // 0x0000004C #19 Reset and clock control
    .word   int_handler_exti7_0      // 0x00000050 #20 EXTI line 0-7
    .word   int_handler_awu          // 0x00000054 #21 Wake-up
    .word   int_handler_dma1_chan1   // 0x00000058 #22 DMA1 Chan1 global
    .word   int_handler_dma1_chan2   // 0x0000005C #23 DMA1 Chan2 global
    .word   int_handler_dma1_chan3   // 0x00000060 #24 DMA1 Chan3 global
    .word   int_handler_dma1_chan4   // 0x00000064 #25 DMA1 Chan4 global
    .word   int_handler_dma1_chan5   // 0x00000068 #26 DMA1 Chan5 global
    .word   int_handler_dma1_chan6   // 0x0000006C #27 DMA1 Chan6 global
    .word   int_handler_dma1_chan7   // 0x00000070 #28 DMA1 Chan7 global
    .word   int_handler_adc          // 0x00000074 #29 ADC global
    .word   int_handler_i2c1_ev      // 0x00000078 #30 i2C1 event
    .word   int_handler_i2c1_er      // 0x0000007C #31 i2C1 error
    .word   int_handler_usart1       // 0x00000080 #32 usart1 global
    .word   int_handler_spi1         // 0x00000084 #33 spi1 global
    .word   int_handler_tim1_brk     // 0x00000088 #34 timer 1 brake
    .word   int_handler_tim1_up      // 0x0000008C #35 timer 1 update
    .word   int_handler_tim1_trg     // 0x00000090 #36 timer 1 trigger
    .word   int_handler_tim1_cc      // 0x00000094 #37 timer 1 capture compare
    .word   int_handler_tim2         // 0x00000098 #38 timer 2 global

    .option rvc;                        // Enable ompressed instructions
    .section    .text.vector_handler, "ax", @progbits
    .weak   int_handler_nmi               
    .weak   int_handler_hard_fault       
    .weak   int_handler_sys_tick           
    .weak   int_handler_sw        
    .weak   int_handler_wwdg     
    .weak   int_handler_pvd     
    .weak   int_handler_flash   
    .weak   int_handler_rcc       
    .weak   int_handler_exti7_0
    .weak   int_handler_awu
    .weak   int_handler_dma1_chan1
    .weak   int_handler_dma1_chan2
    .weak   int_handler_dma1_chan3
    .weak   int_handler_dma1_chan4
    .weak   int_handler_dma1_chan5
    .weak   int_handler_dma1_chan6
    .weak   int_handler_dma1_chan7
    .weak   int_handler_adc       
    .weak   int_handler_i2c1_ev
    .weak   int_handler_i2c1_er
    .weak   int_handler_usart1
    .weak   int_handler_spi1
    .weak   int_handler_tim1_brk  
    .weak   int_handler_tim1_up  
    .weak   int_handler_tim1_trg 
    .weak   int_handler_tim1_cc  
    .weak   int_handler_tim2    

int_handler_nmi:
int_handler_hard_fault:
int_handler_sys_tick:
int_handler_sw:
int_handler_wwdg:
int_handler_pvd:
int_handler_flash:
int_handler_rcc:
int_handler_exti7_0:
int_handler_awu:
int_handler_dma1_chan1:
int_handler_dma1_chan2:
int_handler_dma1_chan3:
int_handler_dma1_chan4:
int_handler_dma1_chan5:
int_handler_dma1_chan6:
int_handler_dma1_chan7:
int_handler_adc:
int_handler_i2c1_ev:
int_handler_i2c1_er:
int_handler_usart1:
int_handler_spi1:
int_handler_tim1_brk:
int_handler_tim1_up:
int_handler_tim1_trg:
int_handler_tim2:
1:
	j 1b
