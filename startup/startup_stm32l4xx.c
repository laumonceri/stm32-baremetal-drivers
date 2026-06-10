// On a Cortex-M MCU, it provides three critical things:
// - The interrupt vector table
// - The reset handler
// - Basic runtime initialization

// This is needed because CPU needs stack + entry point
// without it, CPU jumps to garbage

/* startup_stm32l4xx.c
 *
 * Minimal startup code for STM32L4xx
 * Bare-metal, no CMSIS, no HAL
 */

#include <stdint.h>

/* Symbols provided by the linker script */
extern uint32_t _estack;

extern uint32_t _sidata; /* start of .data in FLASH */
extern uint32_t _sdata;  /* start of .data in RAM */
extern uint32_t _edata;  /* end of .data in RAM */

extern uint32_t _sbss;   /* start of .bss in RAM */
extern uint32_t _ebss;   /* end of .bss in RAM */

int main(void);

/* Function prototypes */
void Reset_Handler(void);
void Default_Handler(void);

/* Interrupt vector table
 * Placed at the start of FLASH (0x08000000)
 */
__attribute__((section(".isr_vector")))
const uint32_t vector_table[] = {
    (uint32_t)&_estack,   /* Initial Main Stack Pointer */
    (uint32_t)Reset_Handler, /* Reset Handler */

    /* Minimal table: all other interrupts go to Default_Handler */
    (uint32_t)Default_Handler, /* NMI */
    (uint32_t)Default_Handler, /* HardFault */
    (uint32_t)Default_Handler, /* MemManage */
    (uint32_t)Default_Handler, /* BusFault */
    (uint32_t)Default_Handler, /* UsageFault */
    0, 0, 0, 0,                /* Reserved */
    (uint32_t)Default_Handler, /* SVCall */
    (uint32_t)Default_Handler, /* Debug Monitor */
    0,                          /* Reserved */
    (uint32_t)Default_Handler, /* PendSV */
    (uint32_t)Default_Handler  /* SysTick */
};

/* Reset handler: runs immediately after reset */
void Reset_Handler(void)
{
    uint32_t *src;
    uint32_t *dst;

    /* Copy .data section from FLASH to RAM */
    src = &_sidata;
    dst = &_sdata;
    while (dst < &_edata) {
        *dst++ = *src++;
    }

    /* Zero initialize .bss section */
    dst = &_sbss;
    while (dst < &_ebss) {
        *dst++ = 0;
    }

    /* Call the main application */
    main();

    /* If main returns, trap the CPU */
    while (1) {
        /* Do nothing */
    }
}

/* Default interrupt handler */
void Default_Handler(void)
{
    while (1) {
        /* Trap CPU on unexpected interrupt */
    }
}

