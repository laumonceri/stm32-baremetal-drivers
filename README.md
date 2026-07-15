# stm32-baremetal-drivers

Bare-metal peripheral drivers for the STM32L4 (Nucleo-L452RE-P), written without HAL or CMSIS. Every driver goes straight to the hardware registers.

**Target:** STM32L452RE, Cortex-M4 @ 16 MHz (HSI16), 512K Flash, 128K RAM  
**Toolchain:** `arm-none-eabi-gcc`, OpenOCD + ST-Link  

---

## Repository layout

```
stm32-baremetal-drivers/
├── drivers/
│   ├── gpio/           GPIO, mode, speed, pull, AF, BSRR atomic writes
│   ├── uart/           USART, polling or interrupt-driven TX/RX (ring buffers), USART1/2
│   ├── i2c/            I2C master, 100 kHz, analog + digital filter
│   ├── spi/            SPI master, full CR1/CR2 config, blocking transfer, see docs/spi.md
│   ├── st7735-display/ ST7735 128x128 SPI LCD: init sequence, FillScreen, DrawImage
│   └── dma/            DMA, planned
├── platform/
│   ├── include/       Public headers for RCC, SysTick, NVIC, EXTI, SYSCFG
│   └── src/           RCC, SysTick, NVIC, EXTI, SYSCFG implementations
├── hw/                Raw register definitions (base addresses, offsets, macros)
├── bsp/               Board-level helpers: LED, button
├── startup/           Reset handler, vector table, .data/.bss init
├── linker.ld          Memory map: FLASH 0x08000000 / RAM 0x20000000
└── examples/            numbered in order of creation
    ├── 01_gpio-blink/         Blink LED on PB13 using GPIO + dummy delay
    ├── 02_button-interrupt/   Press a button and turn on a LED using interrupt
    ├── 03_uart-cli/           Serial CLI, "LED ON" / "LED OFF" commands over USART2
    ├── 04_uart-interrupt/     UART interrupt-driven TX/RX only, see docs/uart.md
    ├── 05_uart-polling/       UART blocking TX/RX only, see docs/uart.md
    ├── 06_spi-loopback/       MOSI/MISO jumper self-test, blinks an LED on byte match
    ├── 07_spi-display-ST7735/ Drives an ST7735 panel: fill screen, draw a 128x128 image
    └── 08_spi-crc-loopback/   SPI CRC self-test over the same MOSI/MISO jumper
```

---

## Architecture

```
       ┌───────┬────────┐
       │   examples/    │  01_gpio-blink ... 08_spi-crc-loopback, numbered in order of creation
       └───────┴────────┘
               |
               │
       ┌───────┬────────┐
       │      BSP       │  board-specific: LED, button pin assignments
       └───────┴────────┘
               │
       ┌──────────────────────────────────────────┐  peripheral logic
       │                 DRIVERS                  │
       │ gpio   uart   i2c   spi   st7735-display │
       └──────────────────────────────────────────┘
               │
       ┌───────┬────────────────────────┐
       │          PLATFORM              │  MCU services
       │ systick  syscfg  rcc nvic exti │
       └───────┴────────────────────────┘
               │
       ┌───────┬────────┐
       │       HW       │  raw register macros, base addresses
       └───────┴────────┘
               │
       ┌───────┬────────┐
       │  STM32L452RE   │
       └────────────────┘
```

---

## Drivers

### GPIO
Full pin configuration via `gpio_pin_cfg_t`: mode, output type, speed, pull-up/down, alternate function. Atomic set/reset through BSRR.

```c
gpio_pin_cfg_t led = {
    .port  = GPIO_PORT_B,
    .pin   = PIN_13,
    .mode  = GPIO_MODE_OUTPUT,
    .otype = GPIO_PUSH_PULL,
    .speed = GPIO_SPEED_LOW,
    .pull  = GPIO_NOPULL,
    .base  = GPIOB_BASE
};
GPIO_ConfigPin(&led);
GPIO_WritePin(&led, GPIO_PIN_SET);
```

### UART
Blocking or interrupt-driven TX/RX, ring-buffered when interrupt-driven. Supports USART1 or USART2 (clock enable auto-selects APB1/APB2 depending on instance). Configurable baud rate, word length, parity, and stop bits. Split across `uart_driver`/`uart_polling`/`uart_interrupt`. Full architecture, diagrams, and API reference in [`docs/uart.md`](docs/uart.md).

```c
UART_Init(&uart2_handle, &BOARD_UART2);
UART_EnableInterrupt(&uart2_handle, IRQ_PRIO_1);

int available = 0;
UART_DataAvailable_RingBuffer(&uart2_handle, &available);
if (available > 0) {
    char c;
    UART_ReadChar_RingBuffer(&uart2_handle, &c);
    UART_WriteChar_RingBuffer(&uart2_handle, c);
```

### I2C
Master-mode driver at 100 kHz with analog filter enabled and 1-cycle digital filter. Timing register computed for 8 MHz PCLK. Clock and GPIO are configured from a single `i2c_device_t` descriptor.

```c
i2c_init(&BOARD_I2C1);
```

### SPI
Master-mode driver, full CR1/CR2 register configuration: baud rate divider, clock polarity/phase, MSB/LSB frame order, CRC, software or hardware slave select, 4-16 bit data size. Blocking `SPI_TransferByte()` for full-duplex transfers, `SPI_Select`/`SPI_Deselect` drive chip-select as a plain GPIO. Hardware-verified with a MOSI/MISO loopback jumper. Full architecture and diagrams in [`docs/spi.md`](docs/spi.md).

```c
SPI_Config(&SPI);
SPI_Select(&SPI);
uint8_t rx = SPI_TransferByte(&SPI, 0xA5);
SPI_Deselect(&SPI);
```

### ST7735 display
Driver for the ST7735 128x128 SPI TFT panel, built on top of the SPI driver. Full init sequence (SLPOUT, frame-rate/power/VCOM/gamma registers, COLMOD, MADCTL, DISPON), `ST7735_FillScreen()` for a solid color, `ST7735_DrawImage()` for a full 128x128 RGB565 pixel buffer.

```c
ST7735_Init(&dev);
ST7735_FillScreen(&dev, 0xF800); // RGB565 red
ST7735_DrawImage(&dev, my_128x128_rgb565_pixels);
```

### Platform
| Module | What it does |
|---|---|
| `rcc` | Enables/resets AHB2 (GPIO) and APB1/APB2 peripheral clocks |
| `systick` | 1 ms tick, `SysTick_Delay()`, `SysTick_GetTick()` |
| `nvic` | `NVIC_EnableIRQ`, `NVIC_SetPriority`, no CMSIS dependency |
| `exti` | Line config, trigger select (rising/falling/both), pending clear |
| `syscfg` | EXTI source routing via `SYSCFG_EXTICR` |

---

## Build & flash

Requirements: `arm-none-eabi-gcc`, `openocd`

Each example is self-contained. `cd` in and build:

```bash
cd examples/<name>   # 01_gpio-blink | 02_button-interrupt | etc
make
make flash
```

Requirements: `picocom` to use any of the uart-* examples

```bash
ls /dev/tty*
picocom --b 115200 /dev/ttyACM0
```


Connect via any serial terminal at **115200 8N1** on the ST-Link virtual COM port.

---

## Running unit tests locally

Quick steps to build and run the host unit tests for the GPIO driver:

Build and run unit tests 

Requirements: `cmocka`

```bash
make ut
```

Run unit tests and get coverage 

Requirements: `lcov`

```bash
make coverage
```

---

## Known limitations

- **I2C clock selection is hardcoded.** `i2c_driver.c` always selects `RCC_SEL_I2C1` regardless of which I2C instance is passed in the descriptor. Using I2C2 or I2C3 will silently misconfigure the clock source.
- **EXTI is hardcoded to line 13.** `exti.c` was written specifically for the user button on PC13.
- **UART instance support is partial.** Only USART1 and USART2 are wired end-to-end (clock enable, validation, IRQ handler). USART3/LPUART1 have registry slots reserved but no IRQ handler yet.
- **UART clock-source frequency lookup only covers fixed sources.** `RCC_GetClockSourceFreq()` resolves HSI16/LSE; PCLK/SYSCLK return 0 since their frequency depends on runtime bus/PLL config this driver doesn't track, which would divide-by-zero the baud rate if selected.
- **SPI is blocking only.** `SPI_TransferByte()` busy-waits on TXE/RXNE; no interrupt or DMA-driven transfer path yet.
- **ST7735 CASET/RASET are hardcoded to 128x128, no offset.** Some panel clones need a column/row RAM offset (commonly 2/1 or 2/3) to address the visible area correctly; not currently configurable per panel variant.
