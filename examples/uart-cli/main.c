#include "led.h"
#include "rcc.h"
#include "stm32_rcc_hw.h"
#include "stm32_uart_hw.h"
#include "uart_driver.h"
#include <stdint.h>

#define LED 13 // PB13

/* runtime state */
static uart_handle_t uart2_handle;

/* hardware description */
static const uart_device_t BOARD_UART2 = {
    .clk = {.apb1 = RCC_APB1ENR1_USART2,
            .clk_sel = RCC_SEL_USART2,
            .clk_src = RCC_CLK_HSI16},

    .uart = {
        .base = USART2_BASE,

        .irq = USART2_IRQn,

        .tx = {.port = GPIO_PORT_A, .pin = 2, .mode = GPIO_MODE_AF, .af = AF_7},

        .rx = {
            .port = GPIO_PORT_A, .pin = 3, .mode = GPIO_MODE_AF, .af = AF_7}}};

static int string_equal(const char *a, const char *b) {
  while (*a && *b) {
    if (*a != *b) {
      return 0;
    }

    a++;
    b++;
  }

  return (*a == '\0' && *b == '\0');
}

int main(void) {
  uart_init(&uart2_handle, &BOARD_UART2);

  uart_write_string(&uart2_handle, "UART READY\r\n");
  int max_len = 32;
  char cmd[max_len];

  Pin_Config led_pin = {.port = GPIO_PORT_B, .pin = LED};
  LED_Init(&led_pin);

  while (1) {
    uart_read_string(&uart2_handle, cmd, max_len);

    if (string_equal(cmd, "LED ON")) {
      uart_write_string(&uart2_handle, "LED ENABLED\r\n");
      LED_On(&led_pin);
    }

    if (string_equal(cmd, "LED OFF")) {
      uart_write_string(&uart2_handle, "LED DISABLED\r\n");
      LED_Off(&led_pin);
    }
  }
}