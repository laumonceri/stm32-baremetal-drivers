#include "rcc.h"
#include "stm32_rcc_hw.h"
#include "stm32_uart_hw.h"
#include "uart_driver.h"
#include "uart_polling.h"

/* Minimal demonstration of the blocking API only, using uart_polling.h.
 * No interrupts, no ring buffer: the whole main loop is parked waiting on
 * hardware flags (TXE/RXNE) the entire time. */

static uart_handle_t uart2_handle;

static const uart_device_t BOARD_UART2 = {
    .clk = {.bus = RCC_UART_BUS_APB1,
            .enr = {.apb1 = RCC_APB1ENR1_USART2},
            .clk_sel = RCC_SEL_USART2,
            .clk_src = RCC_CLK_HSI16},

    .uart = {
        .base = USART2_BASE,
        .irq = USART2_IRQn,
        .word_length = UART_WORD_LENGTH_8B,
        .parity = UART_PARITY_NONE,
        .stop_bits = UART_STOP_1,
        .baudrate = 115200,

        .tx = {.port = GPIO_PORT_A, .pin = PIN_2, .mode = GPIO_MODE_AF, .af = AF_7},
        .rx = {.port = GPIO_PORT_A, .pin = PIN_3, .mode = GPIO_MODE_AF, .af = AF_7}}};

int main(void) {
  RCC_SetSysclk(RCC_SYSCLK_HSI16);

  UART_Init(&uart2_handle, &BOARD_UART2);
  UART_PollWriteString(&uart2_handle, "UART READY (polling)\r\n");

  enum { LINE_MAX = 64 };
  char line[LINE_MAX];

  while (1) {
    UART_PollReadString(&uart2_handle, line, LINE_MAX);

    UART_PollWriteString(&uart2_handle, "\r\necho: ");
    UART_PollWriteString(&uart2_handle, line);
    UART_PollWriteString(&uart2_handle, "\r\n");
  }
}
