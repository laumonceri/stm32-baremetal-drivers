#ifndef UART_POLLING_H
#define UART_POLLING_H

#include "uart_driver.h"

/* Low-level register access (hardware/core layer). */
UART_Status UART_WriteByteRaw(const uart_handle_t *h, char c);
UART_Status UART_ReadByteRaw(const uart_handle_t *h, char *c_received);

/* Polling-based transfer layer. */
UART_Status UART_PollWriteChar(const uart_handle_t *h, char c);
UART_Status UART_PollWriteString(const uart_handle_t *h, const char *s);
UART_Status UART_PollReadChar(const uart_handle_t *h, char *c_received);
UART_Status UART_ReadCharEcho(const uart_handle_t *h, char *c_received);
UART_Status UART_PollReadString(const uart_handle_t *h, char *s_received, int max_len);

#endif
