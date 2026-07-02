#ifndef STM32_NVIC_HW_H
#define STM32_NVIC_HW_H

#include "stm32_mmio.h"

/* ARM Cortex-M Generic User Guide */
#define NVIC_ISER_BASE      0xE000E100UL        /* Interrupt Set-Enable Registers */
#define NVIC_ICER_BASE      0xE000E180UL        /* Interrupt Clear-Enable Registers */
#define NVIC_ISPR_BASE      0xE000E200UL        /* Interrupt Set-Pending Registers */
#define NVIC_ICPR_BASE      0xE000E280UL        /* Interrupt Clear-Pending Registers */
#define NVIC_IABR_BASE      0xE000E300UL        /* Interrupt Active Bit Registers */
#define NVIC_IPR_BASE       0xE000E400UL        /* Interrupt Priority Registers */
#define NVIC_STIR           REG32(0xE000EF00UL) /* Software Trigger Interrupt Register */

/* n[0..7] */
#define NVIC_ISER(n)        REG32(NVIC_ISER_BASE + ((n) * 4U))
#define NVIC_ICER(n)        REG32(NVIC_ICER_BASE + ((n) * 4U))
#define NVIC_ISPR(n)        REG32(NVIC_ISPR_BASE + ((n) * 4U))
#define NVIC_ICPR(n)        REG32(NVIC_ICPR_BASE + ((n) * 4U))
#define NVIC_IABR(n)        REG32(NVIC_IABR_BASE + ((n) * 4U))
#define NVIC_IPR(n)         REG32(NVIC_IPR_BASE  + ((n) * 4U))

/* Helpers */
/* ISER/ICER/ISPR/ICPR/IABR register index */
#define NVIC_REG_INDEX(irq)     ((irq) / 32U)

/* Bit position inside the register */
#define NVIC_REG_BIT(irq)       ((irq) % 32U)

/* IPR register index */
#define NVIC_IPR_INDEX(irq)     ((irq) / 4U)

/* Priority field shift */
#define NVIC_IPR_SHIFT(irq)     (((irq) % 4U) * 8U)




#endif /* STM32_NVIC_HW_H */