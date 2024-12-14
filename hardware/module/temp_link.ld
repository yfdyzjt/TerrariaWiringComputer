MEMORY
{
	RAM (rwx) : ORIGIN = 0, LENGTH = 4M
}

SECTIONS
{
	.text :
	{
		KEEP(*(.text.start));
		KEEP(*(.text.reset));
		*(.text .text.*);
	} > RAM

	.data :
	{
		*(.data .data.* .sdata .sdata.*);
	} > RAM

	.bss : 
	{ 
		*(.bss .bss.* .sbss .sbss.*); 
	} > RAM

	.rodata :
	{
		*(.rodata .rodata.* .srodata .srodata.*);
	} > RAM

	.io :
    {
        *(*.h);
    } > RAM

	_stack_start = ORIGIN(RAM) + LENGTH(RAM);
}
