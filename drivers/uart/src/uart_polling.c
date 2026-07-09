#include "uart_polling.h"
#include "uart_driver_priv.h"

UART_Status UART_WriteByteRaw(const uart_handle_t *h, char c)
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

UART_Status UART_ReadByteRaw(const uart_handle_t *h, char *c_received)
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

UART_Status UART_PollWriteChar(const uart_handle_t *h, char c)
{
    return UART_WriteByteRaw(h, c);
}

UART_Status UART_PollWriteString(const uart_handle_t *h, const char *s)
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
        st = UART_WriteByteRaw(h, *s++);
        if (st != UART_OK) {
            return st;
        }
    }
    return UART_OK;
}

UART_Status UART_PollReadChar(const uart_handle_t *h, char *c_received)
{
    return UART_ReadByteRaw(h, c_received);
}

UART_Status UART_ReadCharEcho(const uart_handle_t *h, char *c_received)
{
    UART_Status st = UART_PollReadChar(h, c_received);
    if (st != UART_OK) {
        return st;
    }

    return UART_WriteByteRaw(h, *c_received);
}

UART_Status UART_PollReadString(const uart_handle_t *h, char *s_received, int max_len)
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
