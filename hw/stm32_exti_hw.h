#ifndef STM32_EXTI_HW_H
#define STM32_EXTI_HW_H

#include "stm32_mmio.h"

#define EXTI_BASE            0x40010400UL

#define EXTI_REG_BLOCK_SIZE   0x20U

/* n = 0,1 for EXTI_IMR1-2 */
#define EXTI_IMR(n)    REG32(EXTI_BASE + ((n) * EXTI_REG_BLOCK_SIZE) + 0x00U)
#define EXTI_EMR(n)    REG32(EXTI_BASE + ((n) * EXTI_REG_BLOCK_SIZE) + 0x04U)
#define EXTI_RTSR(n)   REG32(EXTI_BASE + ((n) * EXTI_REG_BLOCK_SIZE) + 0x08U)
#define EXTI_FTSR(n)   REG32(EXTI_BASE + ((n) * EXTI_REG_BLOCK_SIZE) + 0x0CU)
#define EXTI_SWIER(n)  REG32(EXTI_BASE + ((n) * EXTI_REG_BLOCK_SIZE) + 0x10U)
#define EXTI_PR(n)     REG32(EXTI_BASE + ((n) * EXTI_REG_BLOCK_SIZE) + 0x14U)

/* Helpers */
#define EXTI_REG_INDEX(line)    ((line) / 32U)
#define EXTI_REG_BIT(line)      ((line) % 32U)

#endif /* STM32_EXTI_HW_H */