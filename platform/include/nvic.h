#ifndef NVIC_H
#define NVIC_H

#include "stm32_exti_hw.h"
#include "stm32_nvic_hw.h"
#include <stdint.h>

typedef enum
{
    /* Cortex-M core exceptions */
    Reset_IRQn            = -15,
    NonMaskableInt_IRQn   = -14,
    HardFault_IRQn        = -13,
    MemoryManagement_IRQn = -12,
    BusFault_IRQn         = -11,
    UsageFault_IRQn       = -10,
    SVCall_IRQn           = -5,
    DebugMonitor_IRQn     = -4,
    PendSV_IRQn           = -2,
    SysTick_IRQn          = -1,

    /* STM32L442 external interrupts */
    WWDG_IRQn             = 0,   /**< Window Watchdog */
    PVD_PVM_IRQn          = 1,   /**< PVD through EXTI Line detect */
    RTC_TAMP_STAMP_IRQn   = 2,   /**< RTC Tamper and Timestamp */
    RTC_WKUP_IRQn         = 3,   /**< RTC Wakeup */
    FLASH_IRQn            = 4,   /**< Flash global interrupt */
    RCC_IRQn              = 5,   /**< RCC global interrupt */
    EXTI0_IRQn            = 6,   /**< External interrupt line 0 */
    EXTI1_IRQn            = 7,   /**< External interrupt line 1 */
    EXTI2_IRQn            = 8,   /**< External interrupt line 2 */
    EXTI3_IRQn            = 9,   /**< External interrupt line 3 */
    EXTI4_IRQn            = 10,  /**< External interrupt line 4 */
    DMA1_Channel1_IRQn    = 11,
    DMA1_Channel2_IRQn    = 12,
    DMA1_Channel3_IRQn    = 13,
    DMA1_Channel4_IRQn    = 14,
    DMA1_Channel5_IRQn    = 15,
    DMA1_Channel6_IRQn    = 16,
    DMA1_Channel7_IRQn    = 17,
    ADC1_2_IRQn           = 18,
    CAN1_TX_IRQn          = 19,
    CAN1_RX0_IRQn         = 20,
    CAN1_RX1_IRQn         = 21,
    CAN1_SCE_IRQn         = 22,
    EXTI9_5_IRQn          = 23,
    TIM1_BRK_TIM15_IRQn   = 24,
    TIM1_UP_TIM16_IRQn    = 25,
    TIM1_TRG_COM_TIM17_IRQn = 26,
    TIM1_CC_IRQn          = 27,
    TIM2_IRQn             = 28,
    TIM3_IRQn             = 29,
    TIM4_IRQn             = 30,
    I2C1_EV_IRQn          = 31,
    I2C1_ER_IRQn          = 32,
    I2C2_EV_IRQn          = 33,
    I2C2_ER_IRQn          = 34,
    SPI1_IRQn             = 35,
    SPI2_IRQn             = 36,
    USART1_IRQn           = 37,
    USART2_IRQn           = 38,
    USART3_IRQn           = 39,
    EXTI15_10_IRQn        = 40,
    RTC_ALARM_IRQn        = 41,
    DFSDM1_FLT3_IRQn      = 42,
    TIM8_BRK_IRQn         = 43,
    TIM8_UP_IRQn          = 44,
    TIM8_TRG_COM_IRQn     = 45,
    TIM8_CC_IRQn          = 46,
    ADC3_IRQn             = 47,
    FMC_IRQn              = 48,
    SDMMC1_IRQn           = 49,
    TIM5_IRQn             = 50,
    SPI3_IRQn             = 51,
    UART4_IRQn            = 52,
    UART5_IRQn            = 53,
    TIM6_DAC_IRQn         = 54,
    TIM7_IRQn             = 55,
    DMA2_Channel1_IRQn    = 56,
    DMA2_Channel2_IRQn    = 57,
    DMA2_Channel3_IRQn    = 58,
    DMA2_Channel4_IRQn    = 59,
    DMA2_Channel5_IRQn    = 60,
    DFSDM1_FLT0_IRQn      = 61,
    DFSDM1_FLT1_IRQn      = 62,
    DFSDM1_FLT2_IRQn      = 63,
    COMP_IRQn             = 64,
    LPTIM1_IRQn           = 65,
    LPTIM2_IRQn           = 66,
    USB_FS_IRQn           = 67,
    DMA2_Channel6_IRQn    = 68,
    DMA2_Channel7_IRQn    = 69,
    LPUART1_IRQn          = 70,  /**< Low Power UART 1 */
    QUADSPI_IRQn          = 71,
    I2C3_EV_IRQn          = 72,
    I2C3_ER_IRQn          = 73,
    SAI1_IRQn             = 74,
    SWPMI1_IRQn           = 75,
    TSC_IRQn              = 76,
    LCD_IRQn              = 77,
    AES_IRQn              = 78,
    RNG_IRQn              = 79,
    FPU_IRQn              = 80
} IRQn_Type;

typedef enum
{
    IRQ_PRIO_0 = 0,  /**< Highest priority */
    IRQ_PRIO_1 = 1,
    IRQ_PRIO_2 = 2,
    IRQ_PRIO_3 = 3,
    IRQ_PRIO_4 = 4,
    IRQ_PRIO_5 = 5,
    IRQ_PRIO_6 = 6,
    IRQ_PRIO_7 = 7,
    IRQ_PRIO_8 = 8,
    IRQ_PRIO_9 = 9,
    IRQ_PRIO_10 = 10,
    IRQ_PRIO_11 = 11,
    IRQ_PRIO_12 = 12,
    IRQ_PRIO_13 = 13,
    IRQ_PRIO_14 = 14,
    IRQ_PRIO_15 = 15, /**< Lowest priority */
} IRQn_Priority;

static inline void __disable_irq(void)
{
    __asm volatile ("cpsid i" : : : "memory");
}

static inline void __enable_irq(void)
{
    __asm volatile ("cpsie i" : : : "memory");
}

/* CMSIS access NVIC functions from ARM Cortex-M Generic User Guide */

/**
 * @brief Enables an interrupt or exception.
 * @param IRQn Interrupt number to enable
 */
void NVIC_EnableIRQ(IRQn_Type IRQn);

/**
 * @brief Disables an interrupt or exception.
 * @param IRQn Interrupt number to disable
 */
void NVIC_DisableIRQ(IRQn_Type IRQn);

/**
 * @brief Sets the pending status of interrupt or exception to 1.
 * @param IRQn Interrupt number to set pending
 */
void NVIC_SetPendingIRQ(IRQn_Type IRQn);

/**
 * @brief Clears the pending status of interrupt or exception to 0.
 * @param IRQn Interrupt number to clear pending
 */
void NVIC_ClearPendingIRQ(IRQn_Type IRQn);

/**
 * @brief Reads the pending status of interrupt or exception.
 * @param IRQn Interrupt number to read pending status
 * @return Non-zero value if the pending status is set to 1
 */
uint32_t NVIC_GetPendingIRQ(IRQn_Type IRQn);

/**
 * @brief Sets the priority of an interrupt or exception with configurable
 * priority level to 1.
 * @param IRQn Interrupt number
 * @param priority Priority level (0 = highest, 15 = lowest)
 */
void NVIC_SetPriority(IRQn_Type IRQn, IRQn_Priority priority);

/**
 * @brief Reads the priority of an interrupt or exception with configurable
 * priority level.
 * @param IRQn Interrupt number
 * @return Priority level (0 = highest, 15 = lowest)
 */
uint32_t NVIC_GetPriority(IRQn_Type IRQn);

#endif