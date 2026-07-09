#include "led.h"
#include "rcc.h"
#include "stm32_rcc_hw.h"
#include "stm32_uart_hw.h"
#include "uart_driver.h"
#include <stdint.h>

#define LED 13 // PB13

/* runtime state (non-static: referenced via extern by USART2_IRQHandler) */
uart_handle_t uart2_handle;

/* hardware description */
static const uart_device_t BOARD_UART2 = {
    .clk = {.apb1 = RCC_APB1ENR1_USART2,
            .clk_sel = RCC_SEL_USART2,
            .clk_src = RCC_CLK_HSI16},

    .uart = {
        .base = USART2_BASE,
        .irq = USART2_IRQn,
        .word_length = UART_WORD_LENGTH_9B,
        .parity = UART_PARITY_ODD,
        .stop_bits = UART_STOP_1,
        .baudrate = 115200,

        .tx = {.port = GPIO_PORT_A, .pin = PIN_2, .mode = GPIO_MODE_AF, .af = AF_7},
        .rx = {.port = GPIO_PORT_A, .pin = PIN_3, .mode = GPIO_MODE_AF, .af = AF_7}}};

// static int string_equal(const char *a, const char *b) {
//   while (*a && *b) {
//     if (*a != *b) {
//       return 0;
//     }

//     a++;
//     b++;
//   }

//   return (*a == '\0' && *b == '\0');
// }

int main(void) {
  UART_Init(&uart2_handle, &BOARD_UART2);
  UART_EnableInterrupt(&uart2_handle, IRQ_PRIO_1);

  UART_PollWriteString(&uart2_handle, "UART READY\r\n");

  /* LED Config */
  gpio_pin_cfg_t led_cfg;
  GPIO_Status st =
      GPIO_pin_cfg_init(&led_cfg, GPIO_PORT_B, LED, GPIO_MODE_OUTPUT,
                        GPIO_PUSH_PULL, GPIO_SPEED_LOW, GPIO_NOPULL, AF_0);
  if (st == GPIO_OK) {
    LED_Init(&led_cfg);

    enum { CMD_MAX_LEN = 32 };
    //char cmd[CMD_MAX_LEN];
    //int idx = 0;

    while (1) {
      int available = 0;
      UART_DataAvailable_RingBuffer(&uart2_handle, &available);

      while (available-- > 0) {
        char c;
        UART_ReadChar_RingBuffer(&uart2_handle, &c);
        UART_WriteByteRaw(&uart2_handle, c);

        // if (c == '\r' || c == '\n') {
        //   cmd[idx] = '\0';
        //   idx = 0;

        //   if (string_equal(cmd, "LED ON")) {
        //     UART_PollWriteString(&uart2_handle, "\n");
        //     UART_PollWriteString(&uart2_handle, "LED ENABLED\r\n");
        //     LED_On(&led_cfg);
        //   }

        //   if (string_equal(cmd, "LED OFF")) {
        //     UART_PollWriteString(&uart2_handle, "\n");
        //     UART_PollWriteString(&uart2_handle, "LED DISABLED\r\n");
        //     LED_Off(&led_cfg);
        //   }
        // } else if (idx < CMD_MAX_LEN - 1) {
        //   cmd[idx++] = c;
        // }
      }
    }
  }

  while (1) {
    __asm__("wfi");
  }
}