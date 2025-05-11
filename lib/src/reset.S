#if !defined(LIBCH32_INT_NEST_DEPTH)
#error Need to define int_nest_depth in devices.ini
#endif

#if LIBCH32_INT_NEST_DEPTH == 8
#define INTSYSCR_PMTCFG (0b11 << 2)
#elif LIBCH32_INT_NEST_DEPTH == 4
#define INTSYSCR_PMTCFG (0b10 << 2)
#elif LIBCH32_INT_NEST_DEPTH == 2
#define INTSYSCR_PMTCFG (0b01 << 2)
#else
#define INTSYSCR_PMTCFG (0)
#endif

#define INTSYSCR_INESTEN (1 << 1)

	.section	.text.handle_reset,"ax",@progbits
    .global	handle_reset
	.align	1
handle_reset:
.option push
.option	norelax
	la gp, __global_pointer$
.option	pop
1:
	la sp, _eusrstack
	/*init stack contents*/
	li a2, 0xDEAD
	la a0, _susrstack
	la a1, _eusrstack
	bgeu a0, a1, 2f
1:
	sw a2, (a0)
	addi a0, a0, 4
	bltu a0, a1, 1b
2:
/* Load data section from flash to RAM */
	la a0, _data_lma
	la a1, _data_vma
	la a2, _edata
	bgeu a1, a2, 2f
1:
	lw t0, (a0)
	sw t0, (a1)
	addi a0, a0, 4
	addi a1, a1, 4
	bltu a1, a2, 1b
2:
/* Clear bss section */
	la a0, _sbss
	la a1, _ebss
	bgeu a0, a1, 2f
1:
	sw zero, (a0)
	addi a0, a0, 4
	bltu a0, a1, 1b
2:
/* Configure pipelining and instruction prediction */
    li t0, 0x1f
    csrw 0xbc0, t0

    jal  init_system

/* Enable 2 levels of interrupt nesting*/
	 //li t0, 0x6
	 li t0, INTSYSCR_PMTCFG | INTSYSCR_INESTEN
	 csrw 0x804, t0
	//csrw 0x804, zero
/* Configure the interrupt vector table recognition mode and entry address mode */
 	la t0, _vector_base
    ori t0, t0, 3
	csrw mtvec, t0

/* Enable global interrupt and configure privileged mode */
  li t0, 0x88
  csrw mstatus, t0

	la t0, main
	csrw mepc, t0
	mret
