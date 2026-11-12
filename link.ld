ENTRY(_reset);

MEMORY 
{
	flash(rx) : ORIGIN = 0x08000000, LENGTH = 2048k
	sram(rwx) : ORIGIN = 0x20000000, LENGTH = 128k
} 

STACK_SIZE = 0x2000;
STACK_START = ORIGIN(sram) + LENGTH(sram) - STACK_SIZE;

SECTIONS 
{
	.text : 
	{
		KEEP(*(.vectors))
		*(.text*)
		*(.rodata*)
	} > flash

	.bss (NOLOAD) : 
	{
		. = ALIGN(4);
		_sbss = .;
		*(.bss*)
		*(COMMON)
		. = ALIGN(4);
		_ebss = .;
	} > sram

	.data : 
	{	
		. = ALIGN(4);
		_sdata = .;
		*(.data*) 
		. = ALIGN(4);
		_edata = .;
	} > sram AT > flash

	_sidata = LOADADDR(.data);

	.stack STACK_START (NOLOAD) :
	{
		. = . + STACK_SIZE;
		_estack = .;
	} > sram
}
