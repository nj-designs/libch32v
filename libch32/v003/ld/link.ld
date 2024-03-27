ENTRY( _vector_table )

__stack_size = 512;

PROVIDE( _stack_size = __stack_size );


MEMORY
{
	FLASH (rx)  : ORIGIN = FLASH_ORIGIN, LENGTH = FLASH_LENGTH
	RAM   (xrw) : ORIGIN = RAM_ORIGIN, LENGTH = RAM_LENGTH
}

/* put gp at start of RAM as we only have 2KB */
__global_pointer = RAM_ORIGIN;

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
