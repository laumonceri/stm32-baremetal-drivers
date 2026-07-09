#include "systick.h"

static volatile uint32_t system_tick = 0;

void SysTick_SelectClock(RCC_ClockSource clock) { RCC_CFGR = (1U << clock); }

void SysTick_Handler(void) { system_tick++; }

// The processor has a 24-bit system timer, SysTick, that counts down from
// the reload value to zero, reloads, that is wraps to, the value in the
// SYST_RVR register on the next clock edge, then counts down on subsequent
// clocks.
void SysTick_Init(uint32_t ticks_per_m) {
  // 1. Program reload value.
  SYST_RVR = ticks_per_m - 1;
  // 2. Clear current value.
  SYST_CVR = 0;
  // 3. Program Control and Status register
  SYST_CSR = (SYST_CSR_ENABLE | SYST_CSR_TICKINT | SYST_CSR_CLKSOURCE);
}

uint32_t SysTick_GetTick(void) { return system_tick; }

void SysTick_Delay(uint32_t ms) {
  uint32_t start = system_tick;
  while ((system_tick - start) < ms)
    ;
}