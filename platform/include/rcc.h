#ifndef RCC_H
#define RCC_H

/* enables/disables clocks
resets peripherals
selects clock sources
*/

#include <stdint.h>
#include "stm32_rcc_hw.h"

/* CPU's clock */
typedef enum
{
    RCC_CLK_PCLK  = 0,
    RCC_CLK_SYSCLK,
    RCC_CLK_HSI16,
    RCC_CLK_LSE
} RCC_ClockSource;

/* Peripheral clock*/
typedef enum
{
    RCC_SYSCLK_MSI = 0,
    RCC_SYSCLK_HSI16,
    RCC_SYSCLK_HSE,
    RCC_SYSCLK_PLL
} RCC_SYSCLK_Source;

typedef enum
{
    RCC_SEL_USART1 = 0,
    RCC_SEL_USART2 = 2,
    RCC_SEL_USART3 = 4,
    RCC_SEL_UART4  = 6,
    RCC_SEL_LPUART1 = 10,

    RCC_SEL_I2C1 = 12,
    RCC_SEL_I2C2 = 14,
    RCC_SEL_I2C3 = 16,

    RCC_SEL_LPTIM1 = 18,
    RCC_SEL_LPTIM2 = 20,

    RCC_SEL_SAI1 = 22,
    RCC_SEL_CLK48 = 26,
    RCC_SEL_ADC = 28,
    RCC_SEL_SWPMI1 = 30
} RCC_CCIPR_Field;

typedef enum {
    RCC_AHB1_DMA1  = 0,
    RCC_AHB1_DMA2  = 1,

    RCC_AHB1_FLASH = 8,
    RCC_AHB1_CRC   = 12,
    RCC_AHB1_TSC   = 16,
} RCC_AHB1ENR_Pos;

typedef enum {
    RCC_AHB2_GPIOA = 0,
    RCC_AHB2_GPIOB = 1,
    RCC_AHB2_GPIOC = 2,
    RCC_AHB2_GPIOD = 3,
    RCC_AHB2_GPIOE = 4,
    RCC_AHB2_GPIOF = 5,
    RCC_AHB2_GPIOG = 6,
    RCC_AHB2_GPIOH = 7,

    RCC_AHB2_ADC   = 13,
    RCC_AHB2_AES   = 16,
    RCC_AHB2_HASH  = 17,
    RCC_AHB2_RNG   = 18,
} RCC_AHB2ENR_Pos;

typedef enum {
    RCC_BUS_APB1,
    RCC_BUS_APB2
} RCC_Bus;

typedef enum
{
    RCC_APB1ENR1_TIM2   = 0,
    RCC_APB1ENR1_TIM3   = 1,
    RCC_APB1ENR1_TIM4   = 2,
    RCC_APB1ENR1_TIM5   = 3,
    RCC_APB1ENR1_TIM6   = 4,
    RCC_APB1ENR1_TIM7   = 5,

    RCC_APB1ENR1_WWDG   = 11,
    RCC_APB1ENR1_SPI2   = 14,
    RCC_APB1ENR1_SPI3   = 15,

    RCC_APB1ENR1_USART2 = 17,
    RCC_APB1ENR1_USART3 = 18,
    RCC_APB1ENR1_UART4  = 19,
    RCC_APB1ENR1_UART5  = 20,

    RCC_APB1ENR1_I2C1   = 21,
    RCC_APB1ENR1_I2C2   = 22,
    RCC_APB1ENR1_I2C3   = 23,

    RCC_APB1ENR1_CAN1   = 25,
    RCC_APB1ENR1_CAN2   = 26,

    RCC_APB1ENR1_PWR    = 28,
    RCC_APB1ENR1_DAC1   = 29,
    RCC_APB1ENR1_OPAMP  = 30,
    RCC_APB1ENR1_LPTIM1 = 31,
} RCC_APB1ENR_Pos;

typedef enum
{
    RCC_APB2ENR_SYSCFG = 0,
    RCC_APB2ENR_FW     = 7,
    RCC_APB2ENR_SDMMC1 = 10,
    RCC_APB2ENR_TIM1   = 11,
    RCC_APB2ENR_SPI1   = 12,
    RCC_APB2ENR_USART1 = 14,
    RCC_APB2ENR_TIM15  = 16, 
    RCC_APB2ENR_TIM16  = 17,
    RCC_APB2ENR_SAI1   = 21,
    RCC_APB2ENR_DFSDM1 = 24,
} RCC_APB2ENR_Pos;

typedef union {
    RCC_APB1ENR_Pos apb1;
    RCC_APB2ENR_Pos apb2;
} rcc_enr_t;

/* APB1 peripheral reset register 1 */
typedef enum
{
    RCC_APB1RSTR1_TIM2   = 0,
    RCC_APB1RSTR1_TIM3   = 1,
    RCC_APB1RSTR1_TIM4   = 2,
    RCC_APB1RSTR1_TIM5   = 3,
    RCC_APB1RSTR1_TIM6   = 4,
    RCC_APB1RSTR1_TIM7   = 5,

    RCC_APB1RSTR1_LCD    = 9,

    RCC_APB1RSTR1_SPI2   = 14,
    RCC_APB1RSTR1_SPI3   = 15,

    RCC_APB1RSTR1_USART2 = 17,
    RCC_APB1RSTR1_USART3 = 18,
    RCC_APB1RSTR1_UART4  = 19,
    RCC_APB1RSTR1_UART5  = 20,

    RCC_APB1RSTR1_I2C1   = 21,
    RCC_APB1RSTR1_I2C2   = 22,
    RCC_APB1RSTR1_I2C3   = 23,

    RCC_APB1RSTR1_CRS    = 24,

    RCC_APB1RSTR1_CAN1   = 25,
    RCC_APB1RSTR1_CAN2   = 26,

    RCC_APB1RSTR1_PWR    = 28,
    RCC_APB1RSTR1_DAC1   = 29,
    RCC_APB1RSTR1_OPAMP  = 30,
    RCC_APB1RSTR1_LPTIM1 = 31,
} RCC_APB1RSTR_Pos;



/**
 * @brief Enable the peripheral clock for an AHB1 peripheral
 *
 * Enables the corresponding bit in RCC_AHB1ENR. This must be called before
 * accessing or configuring any peripheral clocked from AHB1 (DMA1/DMA2,
 * FLASH, CRC, TSC), most commonly DMA1/DMA2 before touching their channel
 * registers.
 *
 * @param pos AHB1 peripheral to enable, see RCC_AHB1ENR_Pos
 */
void RCC_EnableAHB1(RCC_AHB1ENR_Pos pos);

/**
 * @brief Enable the peripheral clock for an AHB2 peripheral
 *
 * Enables the corresponding bit in RCC_AHB2ENR. This must be called before
 * accessing or configuring any GPIO registers associated with the port, or
 * any other AHB2 peripheral (ADC, AES, HASH, RNG).
 *
 * @param pos AHB2 peripheral to enable, see RCC_AHB2ENR_Pos
 */
void RCC_EnableAHB2(RCC_AHB2ENR_Pos pos);

/**
 * @brief Enable the peripheral clock for an APB1 peripheral
 *
 * Enables the corresponding bit in RCC_APB1ENR1 (TIM2-7, SPI2/3,
 * USART2/3, UART4/5, I2C1-3, CAN1/2, PWR, DAC1, OPAMP, LPTIM1).
 *
 * @param pos APB1 peripheral to enable, see RCC_APB1ENR_Pos
 */
void RCC_EnableAPB1(RCC_APB1ENR_Pos pos);

/**
 * @brief Enable the peripheral clock for an APB2 peripheral
 *
 * Enables the corresponding bit in RCC_APB2ENR (SYSCFG, TIM1/15/16,
 * SPI1, USART1, SAI1, SDMMC1, DFSDM1).
 *
 * @param pos APB2 peripheral to enable, see RCC_APB2ENR_Pos
 */
void RCC_EnableAPB2(RCC_APB2ENR_Pos pos);

/**
 * @brief Enable a peripheral's clock on whichever APB bus it lives on.
 *
 * Dispatches to RCC_EnableAPB1() or RCC_EnableAPB2() based on bus, so
 * callers with a {bus, enr} pair.
 *
 * @param bus Which APB bus the peripheral is on
 * @param enr Enable position, as enr.apb1 if bus == RCC_BUS_APB1 or
 *            enr.apb2 if bus == RCC_BUS_APB2
 */
void RCC_EnablePeripheralClock(RCC_Bus bus, rcc_enr_t enr);

/**
 * @brief Reset peripheral registers
 * 
 * @param pos values from RCC_APB1RSTR1_Pos
 */
void RCC_ResetAPB1(RCC_APB1RSTR_Pos pos);

void RCC_SetSysclk(RCC_SYSCLK_Source source);
void RCC_CCIPR_SelectClock(RCC_CCIPR_Field periph, RCC_ClockSource src);

/**
 * @brief Resolve a peripheral clock-mux selection to its frequency in Hz.
 *
 * Only sources with a fixed, hardware-defined frequency (HSI16, LSE) can be
 * resolved statically. PCLK/SYSCLK depend on runtime bus/PLL configuration
 * this driver doesn't track, and return 0. Callers must not select them
 * until that tracking exists.
 *
 * @param src clock-mux selection (as passed to RCC_CCIPR_SelectClock)
 * @return frequency in Hz, or 0 if not statically derivable
 */
uint32_t RCC_GetClockSourceFreq(RCC_ClockSource src);

#endif