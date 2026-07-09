#include "uart_driver_priv.h"
#include "uart_interrupt.h"

UART_Status UART_validate_handle(const uart_handle_t *h) {
  if (h == NULL) {
    return UART_ERROR_NULL_HANDLE;
  }

  if (h->dev == NULL) {
    return UART_ERROR_NULL_DEVICE;
  }

  return UART_OK;
}

UART_Status UART_validate_device(const uart_device_t *dev) {
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

static void UART_clock_init(const uart_device_t *dev) {
  if (dev->clk.bus == RCC_UART_BUS_APB2) {
    RCC_EnableAPB2(dev->clk.enr.apb2);
  } else {
    RCC_EnableAPB1(dev->clk.enr.apb1);
  }

  RCC_CCIPR_SelectClock(dev->clk.clk_sel, dev->clk.clk_src);
}

static void UART_gpio_init(const uart_device_t *dev) {
  GPIO_ConfigPin(&dev->uart.tx);
  GPIO_ConfigPin(&dev->uart.rx);
}

static void UART_disable(uint32_t base) { UART_CR1(base) &= ~UART_CR1_UE; }

static UART_Status UART_set_wordlength(uint32_t base, uart_word_length_t len) {
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

static UART_Status UART_set_parity(uint32_t base, uart_parity_t parity) {
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

static void UART_set_baudrate(uint32_t base, uint32_t f_kernel_clock,
                              uint32_t baudrate) {
  UART_BRR(base) = f_kernel_clock / baudrate;
}

static UART_Status UART_set_stopbits(uint32_t base,
                                     uart_stop_bits_t stop_bits) {
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

static void UART_enable(uint32_t base) { UART_CR1(base) |= UART_CR1_UE; }

static void UART_enable_tx(uint32_t base) { UART_CR1(base) |= UART_CR1_TE; }

static void UART_enable_rx(uint32_t base) { UART_CR1(base) |= UART_CR1_RE; }

static void UART_peripheral_init(const uart_device_t *dev) {
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

UART_Status UART_Init(uart_handle_t *h, const uart_device_t *dev) {
  UART_Status st;

  if (h == NULL) {
    return UART_ERROR_NULL_HANDLE;
  }

  st = UART_validate_device(dev);
  if (st != UART_OK) {
    return st;
  }

  if (UART_InstanceInUse(dev, h)) {
    return UART_ERROR_INSTANCE_IN_USE;
  }

  h->dev = dev;
  UART_RegisterHandle(h);

  UART_clock_init(dev);
  UART_gpio_init(dev);
  UART_peripheral_init(dev);

  RingBuffer_Init(&h->rx);
  RingBuffer_Init(&h->tx);

  return UART_OK;
}

UART_Status UART_DeInit(uart_handle_t *h) {
  UART_Status st = UART_validate_handle(h);
  if (st != UART_OK) {
    return st;
  }

  UART_DisableInterrupt(h);
  UART_disable(h->dev->uart.base);
  UART_UnregisterHandle(h);

  h->dev = NULL;

  return UART_OK;
}
