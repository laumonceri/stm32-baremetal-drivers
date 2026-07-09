#include "rcc.h"

void RCC_EnableAHB2(RCC_AHB2ENR_Pos pos) { RCC_AHB2ENR |= (1U << pos); }

void RCC_EnableAPB1(RCC_APB1ENR_Pos pos) { RCC_APB1ENR1 |= (1U << pos); }

void RCC_EnableAPB2(RCC_APB2ENR_Pos pos) { RCC_APB2ENR |= (1U << pos); }

void RCC_ResetAPB1(RCC_APB1RSTR_Pos pos) {
  RCC_APB1RSTR1 |= (1U << pos);
  RCC_APB1RSTR1 &= ~(1U << pos);
}

void RCC_ResetAPB1Peripheral(RCC_APB1RSTR_Pos pos) {
  RCC_APB1RSTR1 |= (1U << pos);
  RCC_APB1RSTR1 &= ~(1U << pos);
}

static uint32_t RCC_EnableSysclk(RCC_SYSCLK_Source source) {
  uint32_t rdy_bit = 0;

  switch (source) {
  case RCC_SYSCLK_MSI:
    RCC_CR |= RCC_CR_MSION;
    rdy_bit = RCC_CR_MSIRDY;
    break;
  case RCC_SYSCLK_HSI16:
    RCC_CR |= RCC_CR_HSION;
    rdy_bit = RCC_CR_HSIRDY;
    break;
  case RCC_SYSCLK_HSE:
    RCC_CR |= RCC_CR_HSEON;
    rdy_bit = RCC_CR_HSERDY;
    break;
  case RCC_SYSCLK_PLL:
    RCC_CR |= RCC_CR_PLLON;
    rdy_bit = RCC_CR_PLLRDY;
    break;
  default:
    // Invalid source, do nothing
    break;
  }

  return rdy_bit;
}

static void RCC_WaitReady(uint32_t rdy_bit) {
  while (!(RCC_CR & rdy_bit))
    ;
}

void RCC_SetSysclk(RCC_SYSCLK_Source source) {
  uint32_t rdy_bit = RCC_EnableSysclk(source);
  RCC_WaitReady(rdy_bit); // enable + wait BEFORE switching
  RCC_CFGR = (RCC_CFGR & ~0x3U) | source;
}

/* Peripherals independent clock configuration register */
void RCC_CCIPR_SelectClock(RCC_CCIPR_Field peripheral, RCC_ClockSource source) {
  RCC_CCIPR &= ~(0x3U << peripheral);
  RCC_CCIPR |= ((source & 0x3U) << peripheral);
}

uint32_t RCC_GetClockSourceFreq(RCC_ClockSource src) {
  switch (src) {
  case RCC_CLK_HSI16:
    return 16000000UL;
  case RCC_CLK_LSE:
    return 32768UL;
  case RCC_CLK_PCLK:
  case RCC_CLK_SYSCLK:
    // TODO: PCLK/SYSCLK depend on runtime bus prescaler / PLL config,
    // which isn't tracked anywhere yet. Returning 0 here means any
    // caller selecting these will silently get a divide-by-zero in
    // baud-rate math. Add real bus-frequency tracking before use.
  default:
    return 0; // not statically derivable
  }
}