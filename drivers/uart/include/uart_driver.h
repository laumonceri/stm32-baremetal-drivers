#ifndef UART_DRIVER_H
#define UART_DRIVER_H

#include <stdint.h>
#include "nvic.h"
#include "gpio.h"
#include "led.h"
#include "stm32_rcc_hw.h"
#include "stm32_uart_hw.h"

/* Buffer sizes */
#define UART_RX_BUFFER_SIZE 128
#define UART_TX_BUFFER_SIZE 128

typedef struct {
    uint32_t base;
    IRQn_Type irq;

    gpio_pin_cfg_t tx;
    gpio_pin_cfg_t rx;
} uart_cfg_t;

typedef struct {
    RCC_APB1ENR_Pos apb1;
    RCC_CCIPR_Field clk_sel;
    uint32_t clk_src;
} rcc_uart_clk_t;

typedef struct {
    rcc_uart_clk_t clk;
    uart_cfg_t uart;
} uart_device_t;

typedef struct {
    volatile uint8_t buffer[UART_RX_BUFFER_SIZE];
    volatile uint16_t head;
    volatile uint16_t tail;
} RingBufferRx;

typedef struct {
    volatile uint8_t buffer[UART_TX_BUFFER_SIZE];
    volatile uint16_t head;
    volatile uint16_t tail;
} RingBufferTx;

typedef struct {
    /* Active UART instance for ISR */
    const uart_device_t *dev;

    // RingBufferRx rx;
    // RingBufferTx tx;
} uart_handle_t;

typedef enum {
    UART_OK = 0,
    UART_ERROR_NULL_HANDLE = 1,
    UART_ERROR_NULL_DEVICE = 2,
    UART_ERROR_INVALID_BASE = 3,
    UART_ERROR_INVALID_CONFIG = 4,
    UART_ERROR_NULL_BUFFER = 5,
    UART_ERROR_INVALID_LENGTH = 6
} UART_Status;


UART_Status UART_Init(uart_handle_t *h, const uart_device_t *dev);

UART_Status UART_WriteChar(const uart_handle_t *h, char c);
UART_Status UART_WriteString(const uart_handle_t *h, const char *s);

// int UART_read_string(char *buf, int max_len);
UART_Status UART_ReadChar(const uart_handle_t *h, char *c_received);
UART_Status UART_ReadString(const uart_handle_t *h, char *s_received, int max_len);
#endif
