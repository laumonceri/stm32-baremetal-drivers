#ifndef LED_H
#define LED_H

#include "gpio.h"

typedef enum {
    LED_OFF,
    LED_ON,
    LED_BLINK
} LED_State;


void LED_Init(const Pin_Config *led);

void LED_On(const Pin_Config *led);

void LED_Off(const Pin_Config *led);

#endif