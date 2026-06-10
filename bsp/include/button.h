#ifndef BUTTON_H
#define BUTTON_H

#include "gpio.h"
#include "stm32_exti_hw.h"
#include <stdbool.h>

typedef enum {
    BUTTON_RELEASED = 0,
    BUTTON_PRESSED
} Button_State;

// configure the pin and any internal pull-up if needed.
void Button_Init(const Pin_Config *button);

/* Event handling */
bool Button_EventPending(void);
void Button_ClearEvent(void);

Button_State Button_GetState(const Pin_Config *button);

void Button_EnableInterrupt(const Pin_Config *button);

#endif