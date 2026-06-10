#include "gpio.h"

#define USER_LED 13 // PB13 -> LED4 USER - GREEN

/**
 * @brief Crude blocking delay (CPU-dependent, approximate)
 * @param iterations Number of loop iterations to delay
 * @note The actual time depends on MCU clock and compiler optimizations.
 */
static void dummy_wait(uint32_t iterations) {
    for (volatile uint32_t i = 0; i < iterations; i++) {
        __asm__("nop"); // Optional: prevent compiler reordering
    }
}

int main(void) {
    // LEDs are outputs from the board
    GPIO_Init(GPIO_PORT_B, USER_LED, GPIO_MODE_OUTPUT, GPIO_NOPULL);

    while (1)
    {
        GPIO_WritePin(GPIO_PORT_B, USER_LED, GPIO_PIN_SET);
        dummy_wait(100000);
        GPIO_WritePin(GPIO_PORT_B, USER_LED, GPIO_PIN_RESET);
        dummy_wait(100000);
    }
    
}