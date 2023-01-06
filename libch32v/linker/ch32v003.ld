ENTRY( _vector_table )

__stack_size = 512;

PROVIDE( _stack_size = __stack_size );

MEMORY
{
	FLASH (rx)  : ORIGIN = 0x00000000, LENGTH = 16K
	RAM   (xrw) : ORIGIN = 0x20000000, LENGTH = 2K
}

/* put gp at start of RAM as we only have 2KB */
__global_pointer = 0x20000000;

SECTIONS
{
	.vector 0x00000000 : {
		/* Vector section *MUST* come 1st */
		KEEP(*(.vector.*))

		. = ALIGN(4); /* Ensure next section is 4 byte aligned */
	} > FLASH

	.text :
	{
		/* All general text sections */
		*(.text.*)

		/* All rodata */
		*(.rodata.*)

		. = ALIGN(4); /* Ensure next section is 4 byte aligned */
	} > FLASH

	.data :
  {
		_sdata = .;

    /* All general data section */
    *(.data.*)
    *(.sdata.*)

		_edata = .;
  }> RAM AT>FLASH

	.bss :
  {
		_sbss = .;

    /* All general data section */
    *(.bss)

		_ebss = .;
  } > RAM

	.stack ORIGIN(RAM) + LENGTH(RAM) - __stack_size :
	{
	    . = ALIGN(4);

	    . = . + __stack_size;

	    _eusrstack = .;
	} >RAM

}


_data_loadaddr = LOADADDR(.data);
_data_size = SIZEOF(.data);
