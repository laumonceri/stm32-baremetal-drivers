#ifndef UART_DRIVER_H
#define UART_DRIVER_H

#include <stdint.h>
#include "nvic.h"
#include "gpio.h"
#include "led.h"
#include "ring_buffer.h"
#include "stm32_rcc_hw.h"
#include "stm32_uart_hw.h"

typedef enum {
    UART_WORD_LENGTH_7B = 0,
    UART_WORD_LENGTH_8B = 1,
    UART_WORD_LENGTH_9B = 2
} uart_word_length_t;

typedef enum
{
    UART_STOP_1 = 0,
    UART_STOP_0_5 = 1,
    UART_STOP_2 = 2,
    UART_STOP_1_5 = 3
} uart_stop_bits_t;

typedef enum {
    UART_PARITY_NONE = 0,
    UART_PARITY_EVEN = 1,
    UART_PARITY_ODD = 2
} uart_parity_t;

typedef struct
{
    uint32_t base;
    IRQn_Type irq;
    uart_word_length_t word_length;
    uart_parity_t parity;
    uart_stop_bits_t stop_bits;
    uint32_t baudrate;

    gpio_pin_cfg_t tx;
    gpio_pin_cfg_t rx;
} uart_config_t;

typedef enum {
    RCC_UART_BUS_APB1,
    RCC_UART_BUS_APB2
} RCC_UART_Bus;

typedef struct {
    RCC_UART_Bus bus;
    union {
        RCC_APB1ENR_Pos apb1;
        RCC_APB2ENR_Pos apb2;
    } enr;
    RCC_CCIPR_Field clk_sel;
    RCC_ClockSource clk_src;
} rcc_uart_clk_t;

typedef struct {
    rcc_uart_clk_t clk;
    uart_config_t uart;
} uart_device_t;

/* Once passed to UART_Init, this handle's address is kept in the driver's
 * IRQ->handle registry and dereferenced from interrupt context at any time
 * until UART_DeInit is called on it. It must therefore have static or
 * global storage duration, never a local/stack variable, or the ISR will
 * dereference a dangling pointer after the function that declared it
 * returns. */
typedef struct {
    /* Active UART instance for ISR */
    const uart_device_t *dev;

    RingBuffer rx;
    RingBuffer tx;
} uart_handle_t;

typedef enum {
    UART_OK = 0,
    UART_ERROR_NULL_HANDLE = 1,
    UART_ERROR_NULL_DEVICE = 2,
    UART_ERROR_INVALID_BASE = 3,
    UART_ERROR_INVALID_CONFIG = 4,
    UART_ERROR_NULL_BUFFER = 5,
    UART_ERROR_INVALID_LENGTH = 6,
    UART_ERROR_INSTANCE_IN_USE = 7,
    UART_ERROR_INVALID_IRQ = 8,
} UART_Status;

/**
 * @brief Initialize a UART instance: clock, GPIO, and peripheral bring-up.
 *
 * Rejects the call if dev's IRQ is already claimed by a different handle
 * (see UART_ERROR_INSTANCE_IN_USE). Call UART_DeInit on the existing
 * handle first to hand ownership over explicitly.
 *
 * @param h   Must have static or global storage duration and remain valid
 *            until UART_DeInit is called (see the uart_handle_t comment).
 * @param dev Hardware/clock/GPIO descriptor for this instance.
 * @return UART_OK, or an error code (see UART_Status).
 */
UART_Status UART_Init(uart_handle_t *h, const uart_device_t *dev);

/**
 * @brief Tear down a UART instance.
 *
 * Disables its interrupt and the peripheral itself, and clears its slot
 * in the internal IRQ->handle registry so the IRQ can be safely re-claimed
 * by a later UART_Init. Invalidates *h. Any further call on h other than
 * a fresh UART_Init will fail validation.
 *
 * @param h Handle previously passed to UART_Init.
 * @return UART_OK, or an error code (see UART_Status).
 */
UART_Status UART_DeInit(uart_handle_t *h);

#endif
