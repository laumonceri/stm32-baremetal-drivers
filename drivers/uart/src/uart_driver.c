#include "uart_driver.h"

#define UART_IRQ_PRIORITY 1
#define BAUDRATE          115200
#define HSI16             16000000 // 16MHz


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

static void UART_set_baudrate(const uart_device_t *dev, uint32_t clock_frequency, uint32_t baudrate)
{
    UART_BRR(dev->uart.base) = clock_frequency / baudrate;
}

static void UART_peripheral_init(const uart_device_t *dev)
{
    UART_CR1(dev->uart.base) &= ~UART_CR1_UE;   // disable before config
   
    UART_set_baudrate(dev, HSI16, BAUDRATE);

    UART_CR1(dev->uart.base) |= UART_CR1_TE | UART_CR1_RE;
    UART_CR1(dev->uart.base) |= UART_CR1_RXNEIE;

    UART_CR1(dev->uart.base) |= UART_CR1_UE;
}

UART_Status UART_init(uart_handle_t *h, const uart_device_t *dev)
{
    h->dev = dev;

    UART_clock_init(dev);
    UART_gpio_init(dev);
    UART_peripheral_init(dev);

    return UART_OK;
}

UART_Status UART_write_char(const uart_handle_t *h, char c)
{
    while (!(UART_ISR(h->dev->uart.base) & UART_ISR_TXE));

    UART_TDR(h->dev->uart.base) = (uint8_t)c;

    return UART_OK;
}

UART_Status UART_write_string(const uart_handle_t *h, const char *s)
{
    while (*s != '\0')
    {
        UART_write_char(h, *s++);
    }
    return UART_OK;
}

UART_Status UART_read_char(const uart_handle_t *h, char *c_received)
{
    /* Wait until RX register contains data */
    while (!(UART_ISR(h->dev->uart.base) & UART_ISR_RXNE));

    /* Read received byte */
    *c_received = (char)UART_RDR(h->dev->uart.base);

    UART_write_char(h, *c_received);

    return UART_OK;
}

UART_Status UART_read_string(const uart_handle_t *h, char *s_received, int max_len)
{
    char c;
    uint8_t idx = 0U;

    while (1)
    {
        UART_read_char(h, &c);

        /* End of line */
        if ((c == '\r') || (c == '\n'))
        {
            s_received[idx] = '\0';

            UART_write_string(h, "\r\n");

            return UART_OK;
        }

         /* Handle backspace / delete */
        if ((c == '\b') || (c == 0x7F))
        {
            if (idx > 0)
            {
                idx--;

                /* Remove char from terminal */
                UART_write_string(h, "\b \b");
            }

            continue;
        }

        /* Prevent buffer overflow */
        if (idx < max_len - 1)
        {
            s_received[idx] = c;
            idx++;
        }
    }
}
