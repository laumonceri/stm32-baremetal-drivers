#ifndef UART_POLLING_H
#define UART_POLLING_H

#include "uart.h"

/* Low-level register access (hardware/core layer). */

/**
 * @brief Block until TXE, then write one byte.
 * @param h Initialized handle.
 * @param c Byte to send.
 * @return UART_OK, or an error code (see UART_Status).
 */
UART_Status UART_WriteByteRaw(const uart_handle_t *h, char c);

/**
 * @brief Block until RXNE, then read one byte.
 * @param h          Initialized handle.
 * @param c_received Out param, the received byte.
 * @return UART_OK, or an error code (see UART_Status).
 */
UART_Status UART_ReadByteRaw(const uart_handle_t *h, char *c_received);

/* Polling-based transfer layer. */

/**
 * @brief Alias for UART_WriteByteRaw.
 * @param h Initialized handle.
 * @param c Byte to send.
 * @return UART_OK, or an error code (see UART_Status).
 */
UART_Status UART_PollWriteChar(const uart_handle_t *h, char c);

/**
 * @brief Block until every byte of a NUL-terminated string is sent.
 * @param h Initialized handle.
 * @param s String to send.
 * @return UART_OK, or an error code (see UART_Status).
 */
UART_Status UART_PollWriteString(const uart_handle_t *h, const char *s);

/**
 * @brief Alias for UART_ReadByteRaw.
 * @param h          Initialized handle.
 * @param c_received Out param, the received byte.
 * @return UART_OK, or an error code (see UART_Status).
 */
UART_Status UART_PollReadChar(const uart_handle_t *h, char *c_received);

/**
 * @brief Block for one byte and immediately write it back out.
 * @param h          Initialized handle.
 * @param c_received Out param, the received (and echoed) byte.
 * @return UART_OK, or an error code (see UART_Status).
 */
UART_Status UART_ReadCharEcho(const uart_handle_t *h, char *c_received);

/**
 * @brief Block, reading and echoing bytes, until '\r'/'\n' or max_len-1
 *        characters have been collected.
 * @param h          Initialized handle.
 * @param s_received Out buffer, NUL-terminated on return.
 * @param max_len    Size of s_received in bytes (must be > 1).
 * @return UART_OK, or an error code (see UART_Status).
 */
UART_Status UART_PollReadString(const uart_handle_t *h, char *s_received, int max_len);

#endif
