#ifndef STM32_TIM_HW_H
#define STM32_TIM_HW_H

/* Advance control timer */
#define TIM1_BASE       0x40012C00UL

/* General-purpose timers */
#define TIM2_BASE       0x40000000UL
#define TIM3_BASE       0x40000400UL
#define TIM15_BASE      0x40014000UL
#define TIM16_BASE      0x40014400UL

/* Basic timer */
#define TIM6_BASE       0x40001000UL

#include "stm32_tim_regs.h"

#endif /* STM32_TIM_HW_H */