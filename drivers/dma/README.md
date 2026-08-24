# DMA Driver

## Overview

The DMA driver provides a direct-memory-access layer for moving data between memory and peripherals without burdening the CPU. In embedded systems, DMA is useful when a peripheral must transfer large amounts of data quickly and predictably.

## Why this matters

DMA is one of the strongest examples of embedded engineering because it separates data movement from software control. Instead of writing byte-by-byte in software, the MCU configures a channel and lets hardware handle the transfer.

This project uses DMA as a core building block for future high-throughput firmware features such as UART TX/RX streaming and SPI traffic acceleration.

## Typical responsibilities

The driver is designed around these concepts:

- channel selection,
- peripheral and memory addresses,
- transfer length,
- direction of movement,
- data size,
- priority level,
- circular vs. normal mode.

## Core design

The configuration object centralizes the transfer settings:

```c
typedef struct {
    RCC_AHB1ENR_Pos peripheral;
    uint32_t base;
    uint32_t channel;
    uint32_t request;
    uint32_t peripheral_address;
    uint32_t memory_address;
    uint32_t data_count;
    dma_enable en;
    dma_data_transfer_direction dir;
    dma_data_size psize;
    dma_data_size msize;
    dma_priority_level pl;
    dma_enable mem2mem;
} dma_config_t;
```

This keeps the config close to the actual STM32 register fields and makes it easier to reason about transfer behavior.

## Example use case

A common use is memory-to-memory copying or peripheral-to-memory transfers for ADC or UART data streaming.

```c
dma_config_t cfg = {
    .base = DMA1_BASE,
    .channel = 1,
    .dir = DMA_DIR_READ_FROM_MEMORY,
    .memory_address = (uint32_t)dst_buffer,
    .peripheral_address = (uint32_t)src_buffer,
    .data_count = 64,
    .psize = DMA_SIZE_32B,
    .msize = DMA_SIZE_32B,
    .pl = DMA_PRIORITY_LVL_HIGH,
    .mem2mem = DMA_ENABLE
};

DMA_Config(&cfg);
```

## Design trade-offs

The driver focuses on the API surface needed for real transfer setup and configuration, without hiding the underlying STM32 DMA model. This is intentional: DMA is low-level and register-intensive, so abstraction must stay clear and explicit.

The trade-off is that the developer needs to understand the STM32 DMA channels and request mapping. In return, the code is efficient, predictable, and much easier to validate in embedded systems work.

## Future improvements

The project already has a good register foundation for DMA, and the next strong steps are:

- DMA enable/disable helpers,
- transfer-complete flag handling,
- UART DMA transfer integration,
- SPI DMA support,
- a fully documented example project.