# UART Driver

## Overview

The UART driver provides a bare-metal serial communication layer for STM32 microcontrollers. It supports both blocking and interrupt-driven communication and is designed to fit the same low-level style as the rest of the repository: direct register access, explicit configuration, and clear error handling.

## Why it matters

UART is one of the most common interfaces in embedded systems. It is used for:

- printing debug logs,
- receiving commands,
- talking to other MCUs,
- bridging to PCs and test equipment,
- creating CLI interfaces and monitoring tools.

## Driver structure

The UART implementation is split into a few specialized components:

- `uart.c` - base UART configuration and setup
- `uart_polling.c` - blocking TX/RX operations
- `uart_interrupt.c` - interrupt-driven RX/TX behavior
- `ring_buffer.c` - FIFO for buffered data transfer

This separation keeps the driver easier to reason about and allows different communication modes without overloading one implementation.

## Core concepts

### Device descriptor

The driver represents an instance with a hardware descriptor and a corresponding handle:

```c
typedef struct {
    uint32_t base;
    IRQn_Type irq;
    uart_word_length_t word_length;
    uart_parity_t parity;
    uart_stop_bits_t stop_bits;
    uint32_t baudrate;
    gpio_pin_cfg_t tx;
    gpio_pin_cfg_t rx;
} uart_config_t;
```

This makes each UART instance explicit and keeps the configuration data close to the actual hardware usage.

### Handle ownership

The interrupt-driven implementation stores a handle pointer in an IRQ registry. That means a handle must remain valid for as long as the UART instance is active. This should be documented and respected in application-level design, especially for static or global handles.

## Typical flow

```c
uart_handle_t uart2_handle;
const uart_device_t BOARD_UART2 = { ... };

UART_Init(&uart2_handle, &BOARD_UART2);
UART_EnableInterrupt(&uart2_handle, IRQ_PRIO_1);

UART_WriteChar_RingBuffer(&uart2_handle, 'H');
UART_WriteChar_RingBuffer(&uart2_handle, 'i');
```

## Features

- configurable baud rate,
- word length selection,
- parity control,
- stop bit selection,
- TX/RX ring buffer support,
- interrupt-driven communication,
- support for board-specific UART configuration tables.

## Design trade-offs

The driver is intentionally direct and minimal: no hidden state machine, no large abstraction layer, no HAL-style framework. This keeps the code predictable and efficient on resource-constrained MCU targets.

The trade-off is that the developer must understand the STM32 USART register model and how RCC/AF mappings work. For bare-metal firmware, that is usually the right choice because it makes debugging and validation straightforward.

## Example use cases

- CLI interfaces over serial terminal
- firmware debug output
- command/response protocols
- logging and monitoring over USB/UART bridge

## Testing

The UART driver has native host-side unit tests and is validated in CI via the repository workflow. This gives a fast feedback loop without requiring live hardware for each change.
