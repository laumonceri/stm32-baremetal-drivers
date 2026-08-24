# ST7735 Display Driver

## Overview

The ST7735 driver is a low-level display interface built on top of the SPI driver. It provides the commands and data transactions needed to initialize a 128x128 TFT display and draw image content efficiently.

## Why this matters

Display drivers are a strong embedded systems example because they combine multiple layers:

- SPI communication,
- GPIO control for DC and reset,
- command sequencing,
- pixel formatting,
- real hardware validation.


## Core responsibilities

The driver implements:

- display reset handling,
- command and parameter transmission,
- screen initialization sequence,
- address window configuration,
- full-screen fill operations,
- image painting using RGB565 pixel data.

## Key API

```c
void ST7735_Init(const st7735_dev_t *dev);
void ST7735_FillScreen(const st7735_dev_t *dev, uint16_t color);
void ST7735_DrawImage(const st7735_dev_t *dev, const uint16_t *pixels);
```

These functions hide the board-specific signaling details while keeping the actual register-level transaction flow clear and direct.

## Hardware interaction model

The display uses a few control pins:

- RES (reset)
- DC (data/command select)
- SPI bus for data transfer

The driver manages the DC line around every command and pixel transfer so the display knows whether the next byte is a command or actual image data.

## Example use

```c
ST7735_Init(&display);
ST7735_FillScreen(&display, 0xF800); // red
ST7735_DrawImage(&display, image_pixels);
```

## Design trade-offs

The display driver is intentionally focused on the specific hardware it was built for: a 128x128 ST7735 module. That keeps the code readable and reliable. The trade-off is that it is not a fully generic driver for every ST7735 variant and panel offset differences.

## Future improvements

Good next steps would be:

- configurable panel geometry and offset handling,
- buffered drawing operations,
- DMA-driven pixel transmission,
- support for multiple panel variants and display orientations.
