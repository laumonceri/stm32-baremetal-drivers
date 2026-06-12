#include "led.h"

void LED_Init(const gpio_pin_cfg_t *led_cfg) {
    (void)GPIO_ConfigPin(led_cfg);
}

void LED_On(const gpio_pin_cfg_t *led_cfg) {
    GPIO_WritePin(led_cfg, GPIO_PIN_SET);
}

void LED_Off(const gpio_pin_cfg_t *led_cfg) {
    GPIO_WritePin(led_cfg, GPIO_PIN_RESET);
}
