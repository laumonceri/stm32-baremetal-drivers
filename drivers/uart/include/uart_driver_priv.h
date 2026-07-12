#ifndef UART_DRIVER_PRIV_H
#define UART_DRIVER_PRIV_H

#include "uart.h"

/**
 * Internal helpers shared between uart.c, uart_polling.c and
 * uart_interrupt.c (not part of the public API)
 */

/**
 * @brief Validate a handle for use.
 * @param h Handle to check.
 * @return UART_OK, UART_ERROR_NULL_HANDLE, or UART_ERROR_NULL_DEVICE.
 */
UART_Status UART_validate_handle(const uart_handle_t *h);

/**
 * @brief Validate a device descriptor's base address, TX/RX port and pin.
 * @param dev Descriptor to check.
 * @return UART_OK, or an error code (see UART_Status).
 */
UART_Status UART_validate_device(const uart_device_t *dev);

/* IRQ->handle registry accessors. Storage and IRQn lookup are private to
 * uart_interrupt.c; uart.c only needs to register/unregister/check
 * ownership from UART_Init/UART_DeInit. */

/**
 * @brief Register a handle in the IRQ->handle registry, keyed by dev->uart.irq.
 * @param h Handle to register (h->dev must already be set).
 */
void UART_RegisterHandle(uart_handle_t *h);

/**
 * @brief Remove a handle from the registry.
 *
 * Only clears the slot if it still points at this exact handle. This
 * guards against a stale call clearing a slot a different handle has
 * since claimed.
 *
 * @param h Handle to unregister.
 */
void UART_UnregisterHandle(uart_handle_t *h);

/**
 * @brief Check whether dev's IRQ is already claimed by a different handle.
 * @param dev Device descriptor naming the IRQ to check.
 * @param h   Handle attempting to claim it.
 * @return Non-zero if a different handle already owns that IRQ.
 */
int UART_InstanceInUse(const uart_device_t *dev, const uart_handle_t *h);

#endif
