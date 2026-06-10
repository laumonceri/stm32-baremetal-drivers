#include "led.h"

void LED_Init(const Pin_Config *led) {
    GPIO_Init(led->port, led->pin, GPIO_MODE_OUTPUT, GPIO_NOPULL);
}

void LED_On(const Pin_Config *led) {
    GPIO_WritePin(led->port, led->pin, GPIO_PIN_SET);
}

void LED_Off(const Pin_Config *led) {
    GPIO_WritePin(led->port, led->pin, GPIO_PIN_RESET);
}
