#ifndef EXTI_H
#define EXTI_H


#include "stm32_exti_hw.h"
#include "gpio.h"
#include "nvic.h"


#include <stdint.h>
#include <stdbool.h>

typedef enum {
    EXTI_TRIGGER_RISING,
    EXTI_TRIGGER_FALLING,
    EXTI_TRIGGER_BOTH
} EXTI_Trigger;

extern volatile bool button_event;

// Configure EXTI line for a specific pin
void EXTI_ConfigLine(const gpio_pin_cfg_t *cfg, EXTI_Trigger trigger);

// Clear pending EXTI line
void EXTI_ClearPending(uint8_t line);

// ISR handlers remain in project/src
void EXTI15_10_IRQHandler(void);

#endif