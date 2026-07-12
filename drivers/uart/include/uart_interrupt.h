#ifndef UART_INTERRUPT_H
#define UART_INTERRUPT_H

#include "uart.h"

/* Interrupt-driven layer. */

/**
 * @brief 
 * 
 * @param irq 
 * @return int 
 */
int UART_RegistryIndex(IRQn_Type irq);

/**
 * @brief Enable RXNEIE and the NVIC line for h's IRQ.
 * @param h        Initialized handle.
 * @param priority NVIC priority (0 = highest, 15 = lowest).
 * @return UART_OK, or an error code (see UART_Status).
 */
UART_Status UART_EnableInterrupt(uart_handle_t *h, IRQn_Priority priority);

/**
 * @brief Disable RXNEIE, TXEIE, and the NVIC line for h's IRQ.
 * @param h Initialized handle.
 * @return UART_OK, or an error code (see UART_Status).
 */
UART_Status UART_DisableInterrupt(uart_handle_t *h);

/**
 * @brief Generic ISR body: services pending RX and TX for one handle.
 *
 * Called via the IRQ->handle registry from USARTx_IRQHandler. Not meant
 * to be called directly by application code.
 *
 * @param h Handle whose peripheral raised the interrupt.
 */
void UART_IRQHandler(uart_handle_t *h);

/* Non-blocking read from the RX ring buffer filled by the ISR. */

/**
 * @brief Get the number of bytes currently buffered in h->rx.
 * @param h         Initialized handle.
 * @param available Out param, byte count (0 if none).
 */
void UART_DataAvailable_RingBuffer(uart_handle_t *h, int *available);

/**
 * @brief Pop one byte from the RX ring buffer.
 * @param h          Initialized handle.
 * @param c_received Out param: the byte, or '\0' if none was available.
 */
void UART_ReadChar_RingBuffer(uart_handle_t *h, char *c_received);

/**
 * @brief Drain the RX ring buffer into s_received until '\r'/'\n', the
 *        buffer runs dry, or max_len-1 characters have been collected.
 *
 * Does not accumulate across calls, restarts at index 0 every call. If a
 * full line hasn't arrived yet, it terminates early rather than waiting;
 * see docs/uart.md for the char-by-char accumulation workaround.
 *
 * @param h          Initialized handle.
 * @param s_received Out buffer, NUL-terminated on return.
 * @param max_len    Size of s_received in bytes.
 */
void UART_ReadString_RingBuffer(uart_handle_t *h, char *s_received, int max_len);

/**
 * @brief Queue one byte for interrupt-driven transmission.
 *
 * Pushes into h->tx and enables TXEIE, which (re)starts draining if it
 * wasn't already running. Do not mix with blocking TX (UART_WriteByteRaw
 * et al.) on the same handle while a queued write is still draining.
 * Both paths drive the same TDR.
 *
 * @param h Initialized handle.
 * @param c Byte to queue.
 * @return UART_OK, or an error code (see UART_Status).
 */
UART_Status UART_WriteChar_RingBuffer(uart_handle_t *h, char c);

/**
 * @brief Queue a NUL-terminated string for interrupt-driven transmission,
 *        the same way as UART_WriteChar_RingBuffer.
 * @param h Initialized handle.
 * @param s String to queue.
 * @return UART_OK, or an error code (see UART_Status).
 */
UART_Status UART_WriteString_RingBuffer(uart_handle_t *h, char *s);

#endif
