#include "uart_interrupt.h"
#include "uart_driver_priv.h"

/* Registered handles, one slot per UART instance this driver supports.
 * Populated in UART_Init (via UART_RegisterHandle), looked up by IRQn from
 * the ISR — an ISR never needs to know a handle's variable name. Must stay
 * sized to match every case in UART_RegistryIndex. */
static uart_handle_t *uart_handle_registry[4];

static int UART_RegistryIndex(IRQn_Type irq)
{
    switch (irq) {
        case USART1_IRQn: return 0;
        case USART2_IRQn: return 1;
        case USART3_IRQn: return 2;
        case LPUART1_IRQn: return 3;
        default:          return -1;
    }
}

void UART_RegisterHandle(uart_handle_t *h)
{
    int idx = UART_RegistryIndex(h->dev->uart.irq);
    if (idx >= 0) {
        uart_handle_registry[idx] = h;
    }
}

void UART_UnregisterHandle(uart_handle_t *h)
{
    int idx = UART_RegistryIndex(h->dev->uart.irq);
    if (idx >= 0 && uart_handle_registry[idx] == h) {
        uart_handle_registry[idx] = NULL;
    }
}

/* The handle must be UART_DeInit'd first, so ownership handoff is always explicit. */
int UART_InstanceInUse(const uart_device_t *dev, const uart_handle_t *h)
{
    int idx = UART_RegistryIndex(dev->uart.irq);
    return idx >= 0 && uart_handle_registry[idx] != NULL && uart_handle_registry[idx] != h;
}

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
    if (RingBuffer_IsFull(&h->rx)) {
        // Buffer is full, discard incoming data
        (void)UART_RDR(h->dev->uart.base); // Read and discard
    } else {
        // BUFFER HAS ROOM: Store the byte and advance the head
        // Read from hardware register
        uint8_t received_byte = (uint8_t)UART_RDR(h->dev->uart.base);
        RingBuffer_Push(&h->rx, received_byte);

    }
}

static void UART_DispatchIRQ(IRQn_Type irq)
{
    int idx = UART_RegistryIndex(irq);
    if (idx >= 0 && uart_handle_registry[idx] != NULL) {
        UART_IRQHandler(uart_handle_registry[idx]);
    }
}

void USART1_IRQHandler(void)
{
    UART_DispatchIRQ(USART1_IRQn);
}

void USART2_IRQHandler(void)
{
    UART_DispatchIRQ(USART2_IRQn);
}

void UART_DataAvailable_RingBuffer(uart_handle_t *h, int *available) {
    if (h == NULL || available == NULL) {
        return;
    }

    if (RingBuffer_IsEmpty(&h->rx)) {
        *available = 0; // No data available
    } else {
        // Calculate the number of bytes available in the buffer
        if (h->rx.head >= h->rx.tail) {
            *available = h->rx.head - h->rx.tail;
        } else {
            *available = RING_BUFFER_SIZE - h->rx.tail + h->rx.head;
        }
    }
}

void UART_ReadChar_RingBuffer(uart_handle_t *h, char *c_received) {
    uint8_t data;
    if (RingBuffer_Pop(&h->rx, &data)) {
        *c_received = (char)data;
    } else {
        *c_received = '\0'; // Indicate no data
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
