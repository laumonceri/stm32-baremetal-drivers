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

static void UART_disable(uint32_t base)
{
    UART_CR1(base) &= ~UART_CR1_UE;
}

static UART_Status UART_set_wordlength(uint32_t base, uart_word_length_t len)
{
    UART_CR1(base) &= ~(UART_CR1_M1 | UART_CR1_M0);

    switch (len) {
        case UART_WORD_LENGTH_7B:
            /* M1=1 M0=0 */
            UART_CR1(base) |= UART_CR1_M1;
            break;
        case UART_WORD_LENGTH_8B:
            // default, do nothing
            break;
        case UART_WORD_LENGTH_9B:
            /* M1=0 M0=1 */
            UART_CR1(base) |= UART_CR1_M0;
            break;
        default:
            return UART_ERROR_INVALID_CONFIG;
    }

    return UART_OK;
}

static void UART_set_baudrate(uint32_t base, uint32_t clock_frequency, uint32_t baudrate)
{
    UART_BRR(base) = clock_frequency / baudrate;
}

static UART_Status UART_set_stopbits(uint32_t base, uart_stop_bits_t stop_bits)
{
    UART_CR2(base) &= ~UART_CR2_STOP_Msk;
    UART_CR2(base) |= stop_bits;

    return UART_OK;
}

static void UART_enable(uint32_t base)
{
    UART_CR1(base) |= UART_CR1_UE;
}

static void UART_enable_tx(uint32_t base)
{
    UART_CR1(base) |= UART_CR1_TE;
}

static void UART_enable_rx(uint32_t base)
{
    UART_CR1(base) |= UART_CR1_RE;
}

static void UART_peripheral_init(const uart_device_t *dev)
{
    UART_disable(dev->uart.base);
    UART_set_wordlength(dev->uart.base, dev->uart.word_length);
    UART_set_baudrate(dev->uart.base, HSI16, BAUDRATE);
    UART_set_stopbits(dev->uart.base, dev->uart.stop_bits);
    UART_enable(dev->uart.base);
    
    UART_enable_tx(dev->uart.base);
    UART_enable_rx(dev->uart.base);
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

    return UART_PollWriteChar(h, *c_received);
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

//////////////////////
UART_Status UART_EnableInterrupt(uart_handle_t *h)
{
    UART_Status st = UART_validate_handle(h);
    if (st != UART_OK) {
        return st;
    }

    UART_CR1(h->dev->uart.base) |= UART_CR1_RXNEIE;

    return UART_OK;
}

UART_Status UART_DisableInterrupt(uart_handle_t *h)
{
    UART_Status st = UART_validate_handle(h);
    if (st != UART_OK) {
        return st;
    }

    UART_CR1(h->dev->uart.base) &= ~UART_CR1_RXNEIE;

    return UART_OK;
}

UART_Status UART_ClearInterruptFlag(uart_handle_t *h)
{
    UART_Status st = UART_validate_handle(h);
    if (st != UART_OK) {
        return st;
    }

    UART_ICR(h->dev->uart.base) |= UART_ISR_RXNE;

    return UART_OK;
}

//////////////////////
//static void ring_buffer_init(RingBufferRx *rb) {
//    rb->head = 0;
//    rb->tail = 0;
//}

//static int ring_buffer_is_empty(const RingBufferRx *rb) {
//    return rb->head == rb->tail;
//}

static int ring_buffer_is_full(const RingBufferRx *rb) {
    return ((rb->head + 1) % UART_RX_BUFFER_SIZE) == rb->tail;
}

// Move head to the next slot
static void ring_buffer_push(RingBufferRx *rb, uint8_t data) {
    if (!ring_buffer_is_full(rb)) {
        rb->buffer[rb->head] = data;
        // Update head index with wrap-around
        rb->head = (rb->head + 1) % UART_RX_BUFFER_SIZE;
    }
}

// Move tail to the next slot
//static int ring_buffer_pop(RingBufferRx *rb, uint8_t *data) {
//    if (!ring_buffer_is_empty(rb)) {
//        *data = rb->buffer[rb->tail];
//        // Update tail index with wrap-around
//        rb->tail = (rb->tail + 1) % UART_RX_BUFFER_SIZE;
//        return 1; // Success
//    }
//    return 0; // Buffer empty
//}

void UART_IRQHandler(uart_handle_t *h) {
    if (ring_buffer_is_full(&h->rx)) {
        // Buffer is full, discard incoming data
        (void)UART_RDR(h->dev->uart.base); // Read and discard
    } else {
        // BUFFER HAS ROOM: Store the byte and advance the head
        // Read from hardware register
        uint8_t received_byte = (uint8_t)UART_RDR(h->dev->uart.base);
        ring_buffer_push(&h->rx, received_byte);

    }
}