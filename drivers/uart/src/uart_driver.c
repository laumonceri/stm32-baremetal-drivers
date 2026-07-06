#include "uart_driver.h"

#define BAUDRATE          115200
#define HSI16             16000000 // 16MHz

static UART_Status UART_validate_handle(const uart_handle_t *h)
{
    if (h == NULL) {
        return UART_ERROR_NULL_HANDLE;
    }

    if (h->dev == NULL) {
        return UART_ERROR_NULL_DEVICE;
    }

    return UART_OK;
}

static UART_Status UART_validate_device(const uart_device_t *dev)
{
    if (dev == NULL) {
        return UART_ERROR_NULL_DEVICE;
    }

    if (dev->uart.base != USART1_BASE && dev->uart.base != USART2_BASE) {
        return UART_ERROR_INVALID_BASE;
    }

    if ((dev->uart.tx.port < GPIO_PORT_A || dev->uart.tx.port > GPIO_PORT_H) ||
        (dev->uart.rx.port < GPIO_PORT_A || dev->uart.rx.port > GPIO_PORT_H)) {
        return UART_ERROR_INVALID_CONFIG;
    }

    if ((dev->uart.tx.pin < PIN_0 || dev->uart.tx.pin > PIN_15) ||
        (dev->uart.rx.pin < PIN_0 || dev->uart.rx.pin > PIN_15)) {
        return UART_ERROR_INVALID_CONFIG;
    }

    return UART_OK;
}

static void UART_clock_init(const uart_device_t *dev)
{
    RCC_EnableAPB1(dev->clk.apb1);

    RCC_HSI16_Enable();

    RCC_CCIPR_SelectClock(dev->clk.clk_sel,
                          dev->clk.clk_src);
}

static void UART_gpio_init(const uart_device_t *dev)
{
    GPIO_ConfigPin(&dev->uart.tx);
    GPIO_ConfigPin(&dev->uart.rx);
}

static void UART_set_baudrate(uint32_t base, uint32_t clock_frequency, uint32_t baudrate)
{
    UART_BRR(base) = clock_frequency / baudrate;
}

static void UART_peripheral_init(const uart_device_t *dev)
{
    UART_CR1(dev->uart.base) &= ~UART_CR1_UE;   // disable before config

    UART_set_baudrate(dev->uart.base, HSI16, BAUDRATE);

    UART_CR1(dev->uart.base) |= UART_CR1_TE | UART_CR1_RE;
    UART_CR1(dev->uart.base) |= UART_CR1_RXNEIE;

    UART_CR1(dev->uart.base) |= UART_CR1_UE;
}

UART_Status UART_Init(uart_handle_t *h, const uart_device_t *dev)
{
    UART_Status st;

    if (h == NULL) {
        return UART_ERROR_NULL_HANDLE;
    }

    st = UART_validate_device(dev);
    if (st != UART_OK) {
        return st;
    }

    h->dev = dev;

    UART_clock_init(dev);
    UART_gpio_init(dev);
    UART_peripheral_init(dev);

    return UART_OK;
}

UART_Status UART_WriteChar(const uart_handle_t *h, char c)
{
    UART_Status st = UART_validate_handle(h);
    if (st != UART_OK) {
        return st;
    }

    st = UART_validate_device(h->dev);
    if (st != UART_OK) {
        return st;
    }

    while (!(UART_ISR(h->dev->uart.base) & UART_ISR_TXE));

    UART_TDR(h->dev->uart.base) = (uint8_t)c;

    return UART_OK;
}

UART_Status UART_WriteString(const uart_handle_t *h, const char *s)
{
    UART_Status st;

    if (h == NULL) {
        return UART_ERROR_NULL_HANDLE;
    }

    if (s == NULL) {
        return UART_ERROR_NULL_BUFFER;
    }

    st = UART_validate_device(h->dev);
    if (st != UART_OK) {
        return st;
    }

    while (*s != '\0') {
        st = UART_WriteChar(h, *s++);
        if (st != UART_OK) {
            return st;
        }
    }
    return UART_OK;
}

UART_Status UART_ReadChar(const uart_handle_t *h, char *c_received)
{
    UART_Status st;

    if (h == NULL) {
        return UART_ERROR_NULL_HANDLE;
    }

    if (c_received == NULL) {
        return UART_ERROR_NULL_BUFFER;
    }

    st = UART_validate_device(h->dev);
    if (st != UART_OK) {
        return st;
    }

    while (!(UART_ISR(h->dev->uart.base) & UART_ISR_RXNE));

    *c_received = (char)UART_RDR(h->dev->uart.base);

    return UART_OK;
}

UART_Status UART_ReadCharEcho(const uart_handle_t *h, char *c_received)
{
    UART_Status st = UART_ReadChar(h, c_received);
    if (st != UART_OK) {
        return st;
    }

    return UART_WriteChar(h, *c_received);
}

UART_Status UART_ReadString(const uart_handle_t *h, char *s_received, int max_len)
{
    UART_Status st;
    char c;
    int idx = 0;

    if (h == NULL) {
        return UART_ERROR_NULL_HANDLE;
    }

    if (s_received == NULL) {
        return UART_ERROR_NULL_BUFFER;
    }

    if (max_len <= 1) {
        return UART_ERROR_INVALID_LENGTH;
    }

    st = UART_validate_device(h->dev);
    if (st != UART_OK) {
        return st;
    }

    while (1) {
        st = UART_ReadCharEcho(h, &c);
        if (st != UART_OK) {
            return st;
        }

        if ((c == '\r') || (c == '\n')) {
            break;
        }

        if (idx < max_len - 1) {
            s_received[idx++] = c;
        }
    }

    s_received[idx] = '\0';
    return UART_OK;
}
