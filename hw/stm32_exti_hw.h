#ifndef STM32_EXTI_HW_H
#define STM32_EXTI_HW_H

#include "stm32_gpio_hw.h"

#define EXTI_BASE            0x40010400UL
#define EXTI_IMR1_OFFSET     0x00
#define EXTI_RTSR1_OFFSET    0x08
#define EXTI_FTSR1_OFFSET    0x0C
#define EXTI_PR1_OFFSET      0x14 // Pending register for EXTI lines 0-31

#define EXTI_IMR1            REG32(EXTI_BASE + EXTI_IMR1_OFFSET)
#define EXTI_RTSR1           REG32(EXTI_BASE + EXTI_RTSR1_OFFSET)
#define EXTI_FTSR1           REG32(EXTI_BASE + EXTI_FTSR1_OFFSET)
#define EXTI_PR1             REG32(EXTI_BASE + EXTI_PR1_OFFSET)

#define SYSCFG_EXTICR1_BASE   0x40010000UL
#define SYSCFG_EXTICR4_OFFSET 0x14
#define SYSCFG_EXTICR4        REG32(SYSCFG_EXTICR1_BASE + SYSCFG_EXTICR4_OFFSET)

/* ARM Cortex-M Generic User Guide */
/* Interrupt Set Enable*/
#define NVIC_ISER_BASE             0xE000E100UL
#define NVIC_ISER0_OFFSET     0x00
#define NVIC_ISER0            REG32(NVIC_ISER_BASE + NVIC_ISER0_OFFSET)
#define NVIC_ISER1_OFFSET     0x04
#define NVIC_ISER1            REG32(NVIC_ISER_BASE + NVIC_ISER1_OFFSET)
#define NVIC_ISER2_OFFSET     0x08
#define NVIC_ISER2            REG32(NVIC_ISER_BASE + NVIC_ISER2_OFFSET)

/* Interurpt priority registers */
#define NVIC_IPR_BASE         0xE000E400UL
#define NVIC_IPR0_OFFSET      0x00
#define NVIC_IPR0             REG32(NVIC_IPR_BASE + NVIC_IPR0_OFFSET)
#define NVIC_IPR1_OFFSET      0x04
#define NVIC_IPR1             REG32(NVIC_IPR_BASE + NVIC_IPR1_OFFSET)
#define NVIC_IPR2_OFFSET      0x08
#define NVIC_IPR2             REG32(NVIC_IPR_BASE + NVIC_IPR2_OFFSET)
#define NVIC_IPR3_OFFSET      0x0C
#define NVIC_IPR3             REG32(NVIC_IPR_BASE + NVIC_IPR3_OFFSET)

/* Interrupt Clear Enable */
#define NVIC_ICER_BASE        0xE000E180UL

#define EXTI15_10_IRQ           40

#endif