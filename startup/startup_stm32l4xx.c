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

void SysTick_Handler(void)          __attribute__((weak, alias("Default_Handler")));

/* External IRQs (weak aliases to Default_Handler) */
void EXTI0_IRQHandler(void)          __attribute__((weak, alias("Default_Handler")));
void EXTI1_IRQHandler(void)          __attribute__((weak, alias("Default_Handler")));
void EXTI2_IRQHandler(void)          __attribute__((weak, alias("Default_Handler")));
void EXTI3_IRQHandler(void)          __attribute__((weak, alias("Default_Handler")));
void EXTI4_IRQHandler(void)          __attribute__((weak, alias("Default_Handler")));
void EXTI9_5_IRQHandler(void)        __attribute__((weak, alias("Default_Handler")));
void EXTI15_10_IRQHandler(void)      __attribute__((weak, alias("Default_Handler")));

void I2C1_EV_IRQHandler(void)       __attribute__((weak, alias("Default_Handler")));

void USART1_IRQHandler(void)         __attribute__((weak, alias("Default_Handler")));
void USART2_IRQHandler(void)         __attribute__((weak, alias("Default_Handler")));
void USART2_IRQHandler(void)         __attribute__((weak, alias("Default_Handler")));
void LPUART1_IRQHandler(void)        __attribute__((weak, alias("Default_Handler")));

/* Interrupt vector table
 * Placed at the start of FLASH (0x08000000)
 */
__attribute__((section(".isr_vector")))
const uint32_t vector_table[] = {
    /* 0-15: Core exceptions */
    (uint32_t)&_estack,       /* 0 Initial Stack Pointer */
    (uint32_t)Reset_Handler,  /* 1 Reset */
    (uint32_t)Default_Handler,/* 2 NMI */
    (uint32_t)Default_Handler,/* 3 HardFault */
    (uint32_t)Default_Handler,/* 4 MemManage */
    (uint32_t)Default_Handler,/* 5 BusFault */
    (uint32_t)Default_Handler,/* 6 UsageFault */
    0, 0, 0, 0,               /* 7-10 Reserved */
    (uint32_t)Default_Handler,/* 11 SVCall */
    (uint32_t)Default_Handler,/* 12 Debug Monitor */
    0,                        /* 13 Reserved */
    (uint32_t)Default_Handler,/* 14 PendSV */
    (uint32_t)SysTick_Handler,/* 15 SysTick */
    (uint32_t)Default_Handler, /* 16 WWDG */
    (uint32_t)Default_Handler, /* 17 PVD_PVM */
    (uint32_t)Default_Handler, /* 18 RTC_TAMP_STAMP */
    (uint32_t)Default_Handler, /* 19 RTC_WKUP */
    (uint32_t)Default_Handler, /* 20 FLASH */
    (uint32_t)Default_Handler, /* 21 RCC */
    (uint32_t)EXTI0_IRQHandler, /* 22 EXTI0 */
    (uint32_t)EXTI1_IRQHandler, /* 23 EXTI1 */
    (uint32_t)EXTI2_IRQHandler, /* 24 EXTI2 */
    (uint32_t)EXTI3_IRQHandler, /* 25 EXTI3 */
    (uint32_t)EXTI4_IRQHandler, /* 26 EXTI4 */
    (uint32_t)Default_Handler, /* 27 DMA1_CH1 */
    (uint32_t)Default_Handler, /* 28 DMA1_CH2 */
    (uint32_t)Default_Handler, /* 29 DMA1_CH3 */
    (uint32_t)Default_Handler, /* 30 DMA1_CH4 */
    (uint32_t)Default_Handler, /* 31 DMA1_CH5 */
    (uint32_t)Default_Handler, /* 32 DMA1_CH6 */
    (uint32_t)Default_Handler, /* 33 DMA1_CH7 */
    (uint32_t)Default_Handler, /* 34 ADC1_2 */
    (uint32_t)Default_Handler, /* 35 CAN1_TX */
    (uint32_t)Default_Handler, /* 36 CAN1_RX0 */
    (uint32_t)Default_Handler, /* 37 CAN1_RX1 */
    (uint32_t)Default_Handler, /* 38 CAN1_SCE */
    (uint32_t)EXTI9_5_IRQHandler, /* 39 EXTI9_5 */
    (uint32_t)Default_Handler, /* 40 TIM1_BRK/TIM15 */
    (uint32_t)Default_Handler, /* 41 TIM1_UP/TIM16 */
    (uint32_t)Default_Handler, /* 42 TIM1_TRG_COM */
    (uint32_t)Default_Handler, /* 43 TIM1_CC */
    (uint32_t)Default_Handler, /* 44 TIM2 */
    (uint32_t)Default_Handler, /* 45 TIM3 */
    (uint32_t)Default_Handler, /* 46 Reserved */
    (uint32_t)I2C1_EV_IRQHandler, /* 47 I2C1_EV */
    (uint32_t)Default_Handler, /* 48 I2C1_ER */
    (uint32_t)Default_Handler, /* 49 I2C2_EV */
    (uint32_t)Default_Handler, /* 50 I2C2_ER */
    (uint32_t)Default_Handler, /* 51 SPI1 */
    (uint32_t)Default_Handler, /* 52 SPI2 */
    (uint32_t)USART1_IRQHandler, /* 53 USART1 */
    (uint32_t)USART2_IRQHandler, /* 54 USART2 */
    (uint32_t)USART3_IRQHandler, /* 55 USART3 */
    (uint32_t)EXTI15_10_IRQHandler, /* 56 EXTI15_10 (PC13) */
    (uint32_t)Default_Handler, /* 57 Reserved */
    (uint32_t)Default_Handler, /* 58 Reserved */
    (uint32_t)Default_Handler, /* 59 Reserved */
    (uint32_t)Default_Handler, /* 60 Reserved */
    (uint32_t)Default_Handler, /* 61 Reserved */
    (uint32_t)Default_Handler, /* 62 Reserved */
    (uint32_t)Default_Handler, /* 63 Reserved */
    (uint32_t)Default_Handler, /* 64 Reserved */
    (uint32_t)Default_Handler, /* 65 Reserved */
    (uint32_t)Default_Handler, /* 66 Reserved */
    (uint32_t)Default_Handler, /* 67 Reserved */
    (uint32_t)Default_Handler, /* 68 Reserved */
    (uint32_t)Default_Handler, /* 69 Reserved */
    (uint32_t)LPUART1_IRQHandler,  /* 70 LPUART1 */
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

