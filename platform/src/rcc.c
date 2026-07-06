#include "rcc.h"

void RCC_EnableAHB2(RCC_AHB2ENR_Pos pos)
{
   RCC_AHB2ENR |= (1U << pos);
}

void RCC_EnableAPB1(RCC_APB1ENR_Pos pos)
{
    RCC_APB1ENR1 |= (1U << pos);
}

void RCC_EnableAPB2(RCC_APB2ENR_Pos pos)
{
    RCC_APB2ENR |= (1U << pos);
}

void RCC_ResetAPB1(RCC_APB1RSTR_Pos pos)
{
    RCC_APB1RSTR1 |= (1U << pos);
    RCC_APB1RSTR1 &= ~(1U << pos);
}

void RCC_ResetAPB1Peripheral(RCC_APB1RSTR_Pos pos)
{
    RCC_APB1RSTR1 |= (1U << pos);
    RCC_APB1RSTR1 &= ~(1U << pos);
}

void RCC_HSI16_Enable(void)
{
    /* ENABLE HSI16 */
    RCC_CR |= (1U << 8);

    /* WAIT HSI16 READY */
    while (!(RCC_CR & (1U << 10)));
}

/* Peripherals independent clock configuration register */
void RCC_CCIPR_SelectClock(RCC_CCIPR_Field peripheral, RCC_ClockSource source)
{
    RCC_CCIPR &= ~(0x3U << peripheral);
    RCC_CCIPR |= ((source & 0x3U) << peripheral);
}