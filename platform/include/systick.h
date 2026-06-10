#ifndef SYSTICK_H
#define SYSTICK_H

#include "stm32_systick_hw.h"
#include "stm32_rcc_hw.h"
#include "rcc.h"
#include <stdint.h>

void SysTick_SelectClock(RCC_ClockSource clock);

// Initialize SysTick for 1ms tick
void SysTick_Init(uint32_t ticks_per_ms);

// Return system uptime in ms
uint32_t SysTick_GetTick(void);

// Non-blocking delay
void SysTick_Delay(uint32_t ms);

#endif
