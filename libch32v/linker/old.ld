ENTRY( _vector_table )

__stack_size = 512;

PROVIDE( _stack_size = __stack_size );

MEMORY
{
	FLASH (rx)  : ORIGIN = 0x00000000, LENGTH = 16K
	RAM   (xrw) : ORIGIN = 0x20000000, LENGTH = 2K
}

SECTIONS
{

	.vector :
	{ 
		_svector = .;
		. = ALIGN(4);
		KEEP(*(SORT_NONE(.vector)))
		. = ALIGN(4);
		_evector = .;
	} >FLASH AT>FLASH

	.text : 
	{
	_stext = .;
	*(.text)
	*(.strings)
	. = ALIGN(4);
	_etext = .;
	} >FLASH AT>FLASH 

	.data : {
		_sdata = .;
		*(.data*)	/* Read-write initialized data */
		*(.sdata*)	/* Read-write initialized data */
		. = ALIGN(4);
    __global_pointer$ = . + 0x800;
		. = ALIGN(8);
		_edata = .;
	} >RAM AT >FLASH
	_data_loadaddr = LOADADDR(.data);

	.bss :
	{
		. = ALIGN(4);
		_sbss = .;
		*(.sbss*)
		*(.gnu.linkonce.sb.*)
		*(.bss*)
		*(.gnu.linkonce.b.*)    
		*(COMMON*)
		. = ALIGN(4);
		_ebss = .;
	} >RAM

	.stack ORIGIN(RAM) + LENGTH(RAM) - __stack_size :
	{
	    _heap_end = . ;
	    . = ALIGN(4);
	    _susrstack = . ;
	    . = . + __stack_size;
	    _eusrstack = .;
	} >RAM 

}
