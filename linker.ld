/* Minimal STM32L4xx linker script */

MEMORY
{
    FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 512K
    RAM   (rwx): ORIGIN = 0x20000000, LENGTH = 128K
}

/* Symbols used by startup.c */
_estack = ORIGIN(RAM) + LENGTH(RAM); /* top of RAM */
_sidata = LOADADDR(.data);
_sdata  = ADDR(.data);
_edata  = ADDR(.data) + SIZEOF(.data);
_sbss   = ADDR(.bss);
_ebss   = ADDR(.bss) + SIZEOF(.bss);

SECTIONS
{
    /* Interrupt vector table */
    .isr_vector :
    {
        KEEP(*(.isr_vector))
    } > FLASH

    /* Program code */
    .text :
    {
        *(.text*)
        *(.rodata*)
    } > FLASH

    /* Initialized data section (.data) */
    .data : AT (_sidata)
    {
        _sdata = .;
        *(.data*)
        _edata = .;
    } > RAM

    /* Zero-initialized data section (.bss) */
    .bss :
    {
        _sbss = .;
        *(.bss*)
        *(COMMON)
        _ebss = .;
    } > RAM
}
