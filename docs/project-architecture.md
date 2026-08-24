# Project Architecture

## Overview

This architecture describes the `stm32-baremetal-drivers` project and shows how hardware access, board logic, and application code are separated into clear layers.

```mermaid
flowchart TD
    A[Application / Example Code] --> B[BSP Layer]
    B --> C[Driver Layer]
    C --> D[Platform Layer]
    D --> E[Hardware Register Definitions]
    E --> F[STM32 MCU]

    C --> C1[GPIO Driver]
    C --> C2[UART Driver]
    C --> C3[SPI Driver]
    C --> C4[ST7735 Display Driver]
    C --> C5[DMA Driver]

    D --> D1[RCC]
    D --> D2[NVIC]
    D --> D3[EXTI]
    D --> D4[SYSCFG]
    D --> D5[SysTick]

    B --> B1[LED]
    B --> B2[Button]

    A --> A1[Examples]
    A1 --> A2[01_gpio-blink]
    A1 --> A3[02_button-interrupt]
    A1 --> A4[03_uart-cli]
    A1 --> A5[07_spi-display-ST7735]
```

## Layer responsibilities

### 1. Application / Example code

This is where the project demonstrates specific use cases such as blinking an LED, reading a button interrupt, driving a UART CLI, or updating a display.

### 2. BSP layer

Board support code provides the mapping from abstract concepts like LED and button to concrete STM32 pins.

Examples:

- LED on PB13
- user button on PC13

### 3. Driver layer

Each driver owns one hardware domain:

- GPIO: pin config and state transitions
- UART: serial RX/TX, ring buffers, interrupts
- SPI: full-duplex byte transfers and device selection
- ST7735: LCD init and framebuffer writes
- DMA: memory-to-memory and peripheral transfers

### 4. Platform layer

This layer provides MCU services that are not tied to one specific peripheral but support the whole system:

- clock management via RCC
- interrupt masking and priorities via NVIC
- external interrupt routing via EXTI and SYSCFG
- timing via SysTick

### 5. Hardware register layer

This is the lowest layer of the design and contains the raw memory-mapped register definitions and base addresses. It is intentionally thin and directly reflects the STM32 reference manual.
