# stm32-baremetal-drivers

Bare-metal peripheral drivers for the STM32L4 (Nucleo-L452RE-P), written without HAL or CMSIS. Every driver goes straight to the hardware registers.

**Target:** STM32L452RE — Cortex-M4 @ 16 MHz (HSI16), 512K Flash, 128K RAM  
**Toolchain:** `arm-none-eabi-gcc`, OpenOCD + ST-Link  

---

## Repository layout

```
stm32-baremetal-drivers/
├── drivers/
│   ├── gpio/          GPIO — mode, speed, pull, AF, BSRR atomic writes
│   ├── uart/          USART — polling TX/RX, ring-buffer scaffolding
│   ├── i2c/           I2C master — 100 kHz, analog + digital filter
│   ├── spi/           SPI — planned
│   └── dma/           DMA — planned
├── platform/
│   ├── rcc.c/h        Clock enable/reset for AHB2 and APB1 peripherals
│   ├── systick.c/h    1 ms timebase, non-blocking delay
│   ├── nvic.c/h       IRQ enable/disable/priority (no CMSIS)
│   └── exti.c/h       External interrupt config + ISR
├── hw/                Raw register definitions (base addresses, offsets, macros)
├── bsp/               Board-level helpers: LED, button
├── startup/           Reset handler, vector table, .data/.bss init
├── linker.ld          Memory map: FLASH 0x08000000 / RAM 0x20000000
└── examples/
    ├── gpio-blink/    Blink LED on PB13 using GPIO + dummy delay
    └── uart-cli/      Serial CLI — "LED ON" / "LED OFF" commands over USART2
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
    .pull  = GPIO_NOPULL
};
GPIO_ConfigPin(&led);
GPIO_WritePin(GPIO_PORT_B, PIN_13, GPIO_PIN_SET);
```

### UART
Polling driver for USART2 at 115200 baud, clocked from HSI16. Handles TX, RX, backspace, and newline detection. Ring buffer structure is defined, interrupt path scaffolded.

```c
uart_init(&uart2_handle, &BOARD_UART2);
uart_write_string(&uart2_handle, "hello\r\n");

char buf[32];
uart_read_string(&uart2_handle, buf, sizeof(buf));
```

### I2C
Master-mode driver at 100 kHz with analog filter enabled and 1-cycle digital filter. Timing register computed for 8 MHz PCLK. Clock and GPIO are configured from a single `i2c_device_t` descriptor.

```c
i2c_init(&BOARD_I2C1);
```

### Platform
| Module | What it does |
|---|---|
| `rcc` | Enables/resets AHB2 (GPIO) and APB1 (UART, I2C, SPI) clocks |
| `systick` | 1 ms tick, `SysTick_Delay()`, `SysTick_GetTick()` |
| `nvic` | `NVIC_EnableIRQ`, `NVIC_SetPriority` — no CMSIS dependency |
| `exti` | Line config, trigger select (rising/falling/both), pending clear |

---

## Build & flash

Requirements: `arm-none-eabi-gcc`, `openocd`

```bash
cd examples/gpio-blink
make
make flash
```

```bash
cd examples/uart-cli
make
make flash
```

Connect via any serial terminal at **115200 8N1** on the ST-Link virtual COM port.

---

## Known limitations

- **I2C clock selection is hardcoded** — `i2c_driver.c` always selects `RCC_SEL_I2C1` regardless of which I2C instance is passed in the descriptor. Using I2C2 or I2C3 will silently misconfigure the clock source.
- **EXTI is hardcoded to line 13** — `exti.c` was written specifically for the user button on PC13. It also bypasses `nvic.h` and writes `NVIC_ISER1`/`NVIC_IPR1` directly, which is inconsistent with the rest of the codebase.
