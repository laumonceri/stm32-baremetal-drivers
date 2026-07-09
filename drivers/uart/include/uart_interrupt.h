#ifndef UART_INTERRUPT_H
#define UART_INTERRUPT_H

#include "uart_driver.h"

/* Interrupt-driven layer. */
UART_Status UART_EnableInterrupt(uart_handle_t *h, IRQn_Priority priority);
UART_Status UART_DisableInterrupt(uart_handle_t *h);
void UART_IRQHandler(uart_handle_t *h);

/* Non-blocking read from the RX ring buffer filled by the ISR. */
void UART_DataAvailable_RingBuffer(uart_handle_t *h, int *available);
void UART_ReadChar_RingBuffer(uart_handle_t *h, char *c_received);
void UART_ReadString_RingBuffer(uart_handle_t *h, char *s_received, int max_len);

UART_Status UART_WriteChar_RingBuffer(uart_handle_t *h, char c);
UART_Status UART_WriteString_RingBuffer(uart_handle_t *h, char *s);

#endif
