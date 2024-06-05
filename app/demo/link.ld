MEMORY
{
    INS_ROM (x) : ORIGIN = 0M, LENGTH = 256K
    DATA_RAM (rw) : ORIGIN = 1M, LENGTH = 64K
    DATA_ROM (r) : ORIGIN = 2M, LENGTH = 256K
}

ENTRY(reset);

SECTIONS
{
    .text :
    {
        KEEP(*(.text.start));
        *(.text.reset);
        *(.text .text.*);
    } > INS_ROM

    .bss :
    {
        *(.bss .bss.*);
    } > DATA_RAM

    .data :
    {
        *(.data .data.*);
    } > DATA_RAM

    .rodata :
    {
        *(.rodata .rodata.*);
    } > DATA_ROM

    _stack_start = ORIGIN(DATA_RAM) + LENGTH(DATA_RAM);
}
