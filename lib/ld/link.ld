ENTRY( _start )

__stack_size = 2048;

PROVIDE( _stack_size = __stack_size );


MEMORY
{
	FLASH (rx) : ORIGIN = FLASH_ORIGIN, LENGTH = FLASH_LENGTH
	RAM   (rw) : ORIGIN = RAM_ORIGIN, LENGTH = RAM_LENGTH
	PERIPHERALS (rw) : ORIGIN = 0x40000000, LENGTH = (0x50050400-0x40000000)
	CORE_PERIPHERALS (rw) : ORIGIN = 0xE0000000, LENGTH = 0x10000
}


SECTIONS
{

	.init :
	{
		_sinit = .;
		. = ALIGN(4);
		KEEP(*(SORT_NONE(.init)))
		. = ALIGN(4);
		_einit = .;
	} >FLASH AT>FLASH

  .vector :
  {
      *(.vector);
	  . = ALIGN(4); /* Used to be 64 */
  } >FLASH AT>FLASH

	.text :
	{
		. = ALIGN(4);
		*(.text)
		*(.text.*)
		*(.rodata)
		*(.rodata*)
		*(.gnu.linkonce.t.*)
		. = ALIGN(4);
	} >FLASH AT>FLASH

	.fini :
	{
		KEEP(*(SORT_NONE(.fini)))
		. = ALIGN(4);
	} >FLASH AT>FLASH

	PROVIDE( _etext = . );
	PROVIDE( _eitcm = . );

	.preinit_array  :
	{
	  PROVIDE_HIDDEN (__preinit_array_start = .);
	  KEEP (*(.preinit_array))
	  PROVIDE_HIDDEN (__preinit_array_end = .);
	} >FLASH AT>FLASH

	.init_array     :
	{
	  PROVIDE_HIDDEN (__init_array_start = .);
	  KEEP (*(SORT_BY_INIT_PRIORITY(.init_array.*) SORT_BY_INIT_PRIORITY(.ctors.*)))
	  KEEP (*(.init_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .ctors))
	  PROVIDE_HIDDEN (__init_array_end = .);
	} >FLASH AT>FLASH

	.fini_array     :
	{
	  PROVIDE_HIDDEN (__fini_array_start = .);
	  KEEP (*(SORT_BY_INIT_PRIORITY(.fini_array.*) SORT_BY_INIT_PRIORITY(.dtors.*)))
	  KEEP (*(.fini_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .dtors))
	  PROVIDE_HIDDEN (__fini_array_end = .);
	} >FLASH AT>FLASH

	.ctors          :
	{
	  /* gcc uses crtbegin.o to find the start of
	     the constructors, so we make sure it is
	     first.  Because this is a wildcard, it
	     doesn't matter if the user does not
	     actually link against crtbegin.o; the
	     linker won't look for a file to match a
	     wildcard.  The wildcard also means that it
	     doesn't matter which directory crtbegin.o
	     is in.  */
	  KEEP (*crtbegin.o(.ctors))
	  KEEP (*crtbegin?.o(.ctors))
	  /* We don't want to include the .ctor section from
	     the crtend.o file until after the sorted ctors.
	     The .ctor section from the crtend file contains the
	     end of ctors marker and it must be last */
	  KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .ctors))
	  KEEP (*(SORT(.ctors.*)))
	  KEEP (*(.ctors))
	} >FLASH AT>FLASH

	.dtors          :
	{
	  KEEP (*crtbegin.o(.dtors))
	  KEEP (*crtbegin?.o(.dtors))
	  KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .dtors))
	  KEEP (*(SORT(.dtors.*)))
	  KEEP (*(.dtors))
	} >FLASH AT>FLASH

	.dalign :
	{
		. = ALIGN(4);
		PROVIDE(_data_vma = .);
	} >RAM AT>FLASH

	.dlalign :
	{
		. = ALIGN(4);
		PROVIDE(_data_lma = .);
	} >FLASH AT>FLASH

	.data :
	{
    	*(.gnu.linkonce.r.*)
    	*(.data .data.*)
    	*(.gnu.linkonce.d.*)
		. = ALIGN(8);
    	PROVIDE( __global_pointer$ = . + 0x800 );
    	*(.sdata .sdata.*)
		*(.sdata2.*)
    	*(.gnu.linkonce.s.*)
    	. = ALIGN(8);
    	*(.srodata.cst16)
    	*(.srodata.cst8)
    	*(.srodata.cst4)
    	*(.srodata.cst2)
    	*(.srodata .srodata.*)
    	. = ALIGN(4);
		PROVIDE( _edata = .);
	} >RAM AT>FLASH

	.bss :
	{
		. = ALIGN(4);
		PROVIDE( _sbss = .);
  	    *(.sbss*)
        *(.gnu.linkonce.sb.*)
		*(.bss*)
     	*(.gnu.linkonce.b.*)
		*(COMMON*)
		. = ALIGN(4);
		PROVIDE( _ebss = .);
	} >RAM AT>FLASH

	PROVIDE( _end = _ebss);
	PROVIDE( end = . );

    .stack ORIGIN(RAM) + LENGTH(RAM) - __stack_size :
    {
        PROVIDE( _heap_end = . );
        PROVIDE( _susrstack = . );
        . = ALIGN(4);
        PROVIDE(_susrstack = . );
        . = . + __stack_size;
        PROVIDE( _eusrstack = .);
    } >RAM

	/* Peripherals ordered by address*/

	.gptm2 0x40000000 (NOLOAD):
	{
		KEEP(*(.gptm2))
	} >PERIPHERALS

	.gptm3 0x40000400 (NOLOAD):
	{
		KEEP(*(.gptm3))
	} >PERIPHERALS

	.gptm4 0x40000800 (NOLOAD):
	{
		KEEP(*(.gptm4))
	} >PERIPHERALS

	.usart2 0x40004400 (NOLOAD):
	{
		KEEP(*(.usart2))
	} >PERIPHERALS

	.i2c1 0x40005400 (NOLOAD):
	{
		KEEP(*(.i2c1))
	} >PERIPHERALS

	.i2c2 0x40005800 (NOLOAD):
	{
		KEEP(*(.i2c2))
	} >PERIPHERALS

	.can1 0x40006400 (NOLOAD):
	{
		KEEP(*(.can1))
	} >PERIPHERALS

	.can1_mb 0x40006580 (NOLOAD):
	{
		KEEP(*(.can1_mb))
	} >PERIPHERALS

	.can1_filter 0x40006600 (NOLOAD):
	{
		KEEP(*(.can1_filter))
	} >PERIPHERALS

	.can2 0x40006800 (NOLOAD):
	{
		KEEP(*(.can2))
	} >PERIPHERALS

	.can2_mb 0x40006980 (NOLOAD):
	{
		KEEP(*(.can2_mb))
	} >PERIPHERALS

  .afio 0x40010000 (NOLOAD):
	{
		KEEP(*(.afio))
	} > PERIPHERALS

	.gpio_a 0x40010800 (NOLOAD):
	{
		KEEP(*(.gpio_a))
	} > PERIPHERALS

	.gpio_b 0x40010C00 (NOLOAD):
	{
		KEEP(*(.gpio_b))
	} > PERIPHERALS


	.gpio_c 0x40011000 (NOLOAD):
	{
		KEEP(*(.gpio_c))
	} > PERIPHERALS


	.gpio_d 0x40011400 (NOLOAD):
	{
		KEEP(*(.gpio_d))
	} > PERIPHERALS

	.gpio_e 0x40011800 (NOLOAD):
	{
		KEEP(*(.gpio_e))
	} > PERIPHERALS

	.adc1 0x40012400 (NOLOAD):
	{
		KEEP(*(.adc1))
	} > PERIPHERALS

	.adc2 0x40012800 (NOLOAD):
	{
		KEEP(*(.adc2))
	} > PERIPHERALS

	.spi1 0x40013000 (NOLOAD):
	{
		KEEP(*(.spi1))
	} >PERIPHERALS

	.adtm1 0x40012C00 (NOLOAD):
	{
		KEEP(*(.adtm1))
	} >PERIPHERALS

	.usart1 0x40013800 (NOLOAD):
	{
		KEEP(*(.usart1))
	} >PERIPHERALS

	.dma1 0x40020000 (NOLOAD):
	{
		KEEP(*(.dma1))
	} > PERIPHERALS

	.rcc 0x40021000 (NOLOAD) :
	{
		KEEP(*(.rcc))
	} >PERIPHERALS

	/* Core Peripherals ordered by address*/
	.pfic 0xE000E000 (NOLOAD) :
	{
		KEEP(*(.pfic))
	} >CORE_PERIPHERALS

	.systick 0xE000F000 (NOLOAD) :
	{
		KEEP(*(.systick))
	} >CORE_PERIPHERALS
}
