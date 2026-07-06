#ifndef RCC_H
#define RCC_H

/* enables/disables clocks
resets peripherals
selects clock sources
*/

#include <stdint.h>
#include "stm32_rcc_hw.h"

#define F_HSI16 16000000UL

typedef enum
{
    RCC_CLK_PCLK  = 0,
    RCC_CLK_SYSCLK,
    RCC_CLK_HSI16,
    RCC_CLK_LSE
} RCC_ClockSource;

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

typedef enum
{
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
 * @brief Enable the peripheral clock for a GPIO port
 *
 * Enables the corresponding AHB2 clock in RCC_AHB2ENR for the selected GPIO port.
 * This must be called before accessing or configuring any GPIO registers
 * associated with the port.
 *
 * @param port GPIO port to enable (GPIO_PORT_A–GPIO_PORT_E)
 */
void RCC_EnableAHB2(RCC_AHB2ENR_Pos pos);

/**
 * @brief 
 * 
 * @param pos 
 */
void RCC_EnableAPB1(RCC_APB1ENR_Pos pos);
void RCC_EnableAPB2(RCC_APB2ENR_Pos pos);

void RCC_ResetAPB1(RCC_APB1RSTR_Pos pos);

/**
 * @brief Reset peripheral registers
 * 
 * @param pos values from RCC_APB1RSTR1_Pos
 */
void RCC_ResetAPB1Peripheral(RCC_APB1RSTR_Pos pos);

void RCC_HSI16_Enable(void);
void RCC_CCIPR_SelectClock(RCC_CCIPR_Field periph, RCC_ClockSource src);

#endif