#include "rcc.h"
#include "stm32_rcc_hw.h"
#include "stm32_uart_hw.h"
#include "uart.h"
#include "uart_interrupt.h"
#include "uart_polling.h"

/* Minimal demonstration of the interrupt-driven API only, using
 * uart_interrupt.h. Both directions run through USART2_IRQHandler in the
 * background, RX fills the rx ring buffer and TX drains the tx ring buffer, the
 * main loop never blocks on hardware, it just moves bytes between the two. */

static uart_handle_t uart2_handle;

static const uart_device_t BOARD_UART2 = {
    .clk = {.bus = RCC_UART_BUS_APB1,
            .enr = {.apb1 = RCC_APB1ENR1_USART2},
            .clk_sel = RCC_SEL_USART2,
            .clk_src = RCC_CLK_HSI16},

    .uart = {.base = USART2_BASE,
             .irq = USART2_IRQn,
             .word_length = UART_WORD_LENGTH_8B,
             .parity = UART_PARITY_NONE,
             .stop_bits = UART_STOP_1,
             .baudrate = 115200,

             .tx = {.port = GPIO_PORT_A,
                    .pin = PIN_2,
                    .mode = GPIO_MODE_AF,
                    .af = AF_7},
             .rx = {.port = GPIO_PORT_A,
                    .pin = PIN_3,
                    .mode = GPIO_MODE_AF,
                    .af = AF_7}}};

int main(void) {
  RCC_SetSysclk(RCC_SYSCLK_HSI16);

  UART_Init(&uart2_handle, &BOARD_UART2);
  UART_EnableInterrupt(&uart2_handle, IRQ_PRIO_1);
  UART_PollWriteString(&uart2_handle, "UART READY (interrupt)\r\n");

  enum { LINE_MAX = 64 };
  char line[LINE_MAX];
  int idx = 0;

  while (1) {
    int available = 0;
    UART_DataAvailable_RingBuffer(&uart2_handle, &available);

    while (available-- > 0) {
      char c;
      UART_ReadChar_RingBuffer(&uart2_handle, &c);
      UART_WriteChar_RingBuffer(&uart2_handle, c); // live echo while typing

      if (c == '\r' || c == '\n') {
        line[idx] = '\0';
        idx = 0;

        UART_WriteString_RingBuffer(&uart2_handle, "\r\n");
        UART_WriteString_RingBuffer(&uart2_handle, line);
        UART_WriteString_RingBuffer(&uart2_handle, "\r\n");
      } else if (idx < LINE_MAX - 1) {
        line[idx++] = c;
      }
    }
  }
}
