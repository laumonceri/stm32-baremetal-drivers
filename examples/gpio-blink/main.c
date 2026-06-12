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
  gpio_pin_cfg_t cfg;
  GPIO_Status st =
      gpio_pin_cfg_init(&cfg, GPIO_PORT_B, USER_LED, GPIO_MODE_OUTPUT,
                        GPIO_PUSH_PULL, GPIO_SPEED_LOW, GPIO_NOPULL, AF_0);
  if (st == GPIO_OK) {
    GPIO_ConfigPin(&cfg);

    while (1) {
      GPIO_WritePin(&cfg, GPIO_PIN_SET);
      dummy_wait(100000);
      GPIO_WritePin(&cfg, GPIO_PIN_RESET);
      dummy_wait(100000);
    }
  }

  // If initialization failed, stay here (could add error handling/logging)
  while (1) {
    __asm__("wfi"); // wait for interrupt
  }
}