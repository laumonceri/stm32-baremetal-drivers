# stm32-baremetal-drivers

Bare-metal peripheral drivers for the STM32L4 (Nucleo-L452RE-P), written without HAL or CMSIS. Every driver goes straight to the hardware registers.

**Target:** STM32L452RE, Cortex-M4 @ 16 MHz (HSI16), 512K Flash, 128K RAM  
**Toolchain:** `arm-none-eabi-gcc`, OpenOCD + ST-Link  

---

## Repository layout

```
stm32-baremetal-drivers/
├── drivers/
│   ├── gpio/          GPIO, mode, speed, pull, AF, BSRR atomic writes
│   ├── uart/          USART, polling or interrupt-driven TX/RX (ring buffers), USART1/2
│   ├── i2c/           I2C master, 100 kHz, analog + digital filter
│   ├── spi/           SPI, planned
│   └── dma/           DMA, planned
├── platform/
│   ├── include/       Public headers for RCC, SysTick, NVIC, EXTI, SYSCFG
│   └── src/           RCC, SysTick, NVIC, EXTI, SYSCFG implementations
├── hw/                Raw register definitions (base addresses, offsets, macros)
├── bsp/               Board-level helpers: LED, button
├── startup/           Reset handler, vector table, .data/.bss init
├── linker.ld          Memory map: FLASH 0x08000000 / RAM 0x20000000
└── examples/
    ├── button-interrupt/ Press a button and turn on a LED using interrupt
    ├── gpio-blink/       Blink LED on PB13 using GPIO + dummy delay
    ├── uart-polling/     UART blocking TX/RX only, see docs/uart.md
    ├── uart-interrupt/   UART interrupt-driven TX/RX only, see docs/uart.md
    └── uart-cli/         Serial CLI, "LED ON" / "LED OFF" commands over USART2
```

---

## Architecture

```
       ┌───────┬────────┐
       │   examples/    │  button-interrupt, gpio-blink, uart-polling, uart-interrupt, uart-cli
       └───────┴────────┘
               |
               │
       ┌───────┬────────┐
       │      BSP       │  board-specific: LED, button pin assignments
       └───────┴────────┘
               │
       ┌───────┬────────────────────────┐
       │           DRIVERS              │  peripheral logic
       │   gpio     uart     i2c   spi  │
       └───────┴────────────────────────┘
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
cd examples/<name>   # button-interrupt | gpio-blink | uart-polling | uart-interrupt | uart-cli
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
