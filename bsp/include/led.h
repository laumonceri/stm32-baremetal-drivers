#ifndef LED_H
#define LED_H

#include "gpio.h"

typedef enum {
    LED_OFF,
    LED_ON,
    LED_BLINK
} LED_State;


void LED_Init(const gpio_pin_cfg_t *led_cfg);

void LED_On(const gpio_pin_cfg_t *led_cfg);

void LED_Off(const gpio_pin_cfg_t *led_cfg);

#endif