#include "uart_driver.h"

static void ring_buffer_init(RingBufferRx *rb);

/* Registered handles, one slot per UART instance this driver supports
 * (matches UART_validate_device's base-address whitelist). Populated in
 * UART_Init, looked up by IRQn from the ISR — an ISR never needs to know a
 * handle's variable name. */
static uart_handle_t *uart_handle_registry[2];

static int UART_RegistryIndex(IRQn_Type irq)
{
    switch (irq) {
        case USART1_IRQn: return 0;
        case USART2_IRQn: return 1;
        default:          return -1;
    }
}

static void UART_RegisterHandle(uart_handle_t *h)
{
    int idx = UART_RegistryIndex(h->dev->uart.irq);
    if (idx >= 0) {
        uart_handle_registry[idx] = h;
    }
}

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
            /* M1=0 M0=0 */
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

static UART_Status UART_set_parity(uint32_t base, uart_parity_t parity)
{
    UART_CR1(base) &= ~UART_CR1_PCE; // Clear parity enable
    UART_CR1(base) &= ~UART_CR1_PS;  // Clear parity selection

    switch (parity) {
        case UART_PARITY_NONE:
            // default, do nothing
            break;
        case UART_PARITY_EVEN:
            UART_CR1(base) |= UART_CR1_PCE; // Enable parity
            break;
        case UART_PARITY_ODD:
            UART_CR1(base) |= UART_CR1_PCE; // Enable parity
            UART_CR1(base) |= UART_CR1_PS;  // Set odd parity
            break;
        default:
            return UART_ERROR_INVALID_CONFIG;
    }

    return UART_OK;
}

static void UART_set_baudrate(uint32_t base, uint32_t f_kernel_clock, uint32_t baudrate)
{
    UART_BRR(base) = f_kernel_clock / baudrate;
}

static UART_Status UART_set_stopbits(uint32_t base, uart_stop_bits_t stop_bits)
{
    UART_CR2(base) &= ~UART_CR2_STOP_Msk;

    switch (stop_bits) {
        case UART_STOP_1:
        case UART_STOP_0_5:
        case UART_STOP_2:
        case UART_STOP_1_5:
            UART_CR2(base) |= UART_CR2_STOP(stop_bits);
            break;
        default:
            return UART_ERROR_INVALID_CONFIG;
    }

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
    UART_set_parity(dev->uart.base, dev->uart.parity);
    uint32_t frequency = RCC_GetClockSourceFreq(dev->clk.clk_src);
    UART_set_baudrate(dev->uart.base, frequency, dev->uart.baudrate);
    UART_set_stopbits(dev->uart.base, dev->uart.stop_bits);
    UART_enable(dev->uart.base);
    
    UART_enable_tx(dev->uart.base);
    UART_enable_rx(dev->uart.base);
}

static void ring_buffer_init(RingBufferRx *rb) {
   rb->head = 0;
   rb->tail = 0;
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
    UART_RegisterHandle(h);

    UART_clock_init(dev);
    UART_gpio_init(dev);
    UART_peripheral_init(dev);

    ring_buffer_init(&h->rx);

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

//////////////////////
static int ring_buffer_is_empty(const RingBufferRx *rb) {
   return rb->head == rb->tail;
}

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

void UART_DataAvailable_RingBuffer(uart_handle_t *h, int *available) {
    if (h == NULL || available == NULL) {
        return;
    }

    if (ring_buffer_is_empty(&h->rx)) {
        *available = 0; // No data available
    } else {
        // Calculate the number of bytes available in the buffer
        if (h->rx.head >= h->rx.tail) {
            *available = h->rx.head - h->rx.tail;
        } else {
            *available = UART_RX_BUFFER_SIZE - h->rx.tail + h->rx.head;
        }
    }
}

void UART_ReadChar_RingBuffer(uart_handle_t *h, char *c_received) {
    if (ring_buffer_is_empty(&h->rx)) {
        // Buffer is empty, no data to read
        *c_received = '\0'; // Indicate no data
    } else {
        // BUFFER HAS DATA: Read the byte and advance the tail
        *c_received = h->rx.buffer[h->rx.tail];
        h->rx.tail = (h->rx.tail + 1) % UART_RX_BUFFER_SIZE;
    }
}

void UART_ReadString_RingBuffer(uart_handle_t *h, char *s_received, int max_len) {
    int idx = 0;
    char c;

    while (idx < max_len - 1) {
        UART_ReadChar_RingBuffer(h, &c);
        if (c == '\0') {
            break; // No more data in the buffer
        }

        s_received[idx++] = c;

        if (c == '\r' || c == '\n') {
            break; // End of string
        }
    }

    s_received[idx] = '\0'; // Null-terminate the string
}

//////////////////////
static void UART_SetInterruptEnable(uart_handle_t *h, int enable)
{
    if (enable) {
        UART_CR1(h->dev->uart.base) |= UART_CR1_RXNEIE;
    } else {
        UART_CR1(h->dev->uart.base) &= ~UART_CR1_RXNEIE;
    }
}

UART_Status UART_EnableInterrupt(uart_handle_t *h, IRQn_Priority priority)
{
    UART_Status st = UART_validate_handle(h);
    if (st != UART_OK) {
        return st;
    }

    if (priority < IRQ_PRIO_0 || priority > IRQ_PRIO_15) {
        return UART_ERROR_INVALID_CONFIG;
    }

    UART_SetInterruptEnable(h, 1);

    NVIC_ClearPendingIRQ(h->dev->uart.irq);
    NVIC_SetPriority(h->dev->uart.irq, priority);
    NVIC_EnableIRQ(h->dev->uart.irq);

    return UART_OK;
}

UART_Status UART_DisableInterrupt(uart_handle_t *h)
{
    UART_Status st = UART_validate_handle(h);
    if (st != UART_OK) {
        return st;
    }

    UART_SetInterruptEnable(h, 0);
    NVIC_DisableIRQ(h->dev->uart.irq);

    return UART_OK;
}

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

static void UART_DispatchIRQ(IRQn_Type irq)
{
    int idx = UART_RegistryIndex(irq);
    if (idx >= 0 && uart_handle_registry[idx] != NULL) {
        UART_IRQHandler(uart_handle_registry[idx]);
    }
}

void USART2_IRQHandler(void)
{
    UART_DispatchIRQ(USART2_IRQn);
}