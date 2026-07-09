#ifndef UART_DRIVER_PRIV_H
#define UART_DRIVER_PRIV_H

#include "uart_driver.h"

/**
 * Internal helpers shared between uart_driver.c, uart_polling.c and
 * uart_interrupt.c. Not part of the public API — application code should
 * never include this header; use uart_driver.h / uart_polling.h /
 * uart_interrupt.h instead.
 */

UART_Status UART_validate_handle(const uart_handle_t *h);
UART_Status UART_validate_device(const uart_device_t *dev);

/* IRQ->handle registry accessors. Storage and IRQn lookup are private to
 * uart_interrupt.c; uart_driver.c only needs to register/unregister/check
 * ownership from UART_Init/UART_DeInit. */
void UART_RegisterHandle(uart_handle_t *h);
void UART_UnregisterHandle(uart_handle_t *h);
int UART_InstanceInUse(const uart_device_t *dev, const uart_handle_t *h);

#endif
