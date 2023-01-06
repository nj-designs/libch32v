  .section  .text.reset_handler, "ax", @progbits
  .globl  reset_handler
	.align    1
reset_handler:
  .option push
  .option norelax
    // Initialize global pointer
    la		gp, __global_pointer
  .option pop

  la sp, _eusrstack

  // Clear the bss segment
  la      a0, _sbss      // a0 = start zeroing memory here
  la      a1, _ebss      // a1 = end zeroing here-1
clear_bss:
  bgeu    a0, a1, done_bss      // while (!(a0 >= a1))
  sb      x0, 0(a0)            //   *a0 = 0;
  addi    a0, a0, 1             //   ++a0
  beq     x0, x0, clear_bss
done_bss:

  // Init data segment
  la     a0, _sdata           // data (dest) start
  la     a1, _edata           // data (dest) end
  la     a2, _data_loadaddr   // text (src) start
init_data_loop:
  bgeu    a0, a1, init_data_done
  lw     t1, 0(a2)
  addi   a2, a2, 4
  sw     t1, 0(a0)
  addi   a0, a0, 4
  beq    zero, zero, init_data_loop
init_data_done:

  // See https://five-embeddev.com/riscv-isa-manual/latest/priv-csrs.html

  // CH32V003RM-EN - 6.5.3.2 Exception entry base address register (MTVEC)
  la t0, _vector_table
  ori t0, t0, 3
  csrw mtvec, t0

  li t0, 0x80
  csrw mstatus, t0

  // CH32V003RM-EN - 6.5.3.1 Interrupt system control register (INTSYSCR)
  // li t0, 0x3
  csrw 0x804, zero

  // Finish settting up
  jal   SystemInit

  // mret will return to mepc (i.e. jump to main)
  // la t0, main
  // csrw mepc, t0
  // mret
  jal main


  # abort execution here
  ebreak

