GPIO Driver
===========

Bare-metal GPIO abstraction for the STM32L4 family.

Overview
--------
This driver provides the foundation for all pin-level control in the project. It configures GPIO ports and pins directly through the STM32 hardware registers, without using HAL or CMSIS. The goal is to keep the API simple, explicit, and easy to validate on real hardware and in host-side unit tests.

The driver is used by the board support layer, UART, SPI, and display logic. In practice, it is the lowest-level building block that makes the rest of the firmware possible.

Header
------
See `drivers/gpio/include/gpio.h` for the public API and data structures.

Key types
---------
- `gpio_pin_cfg_t` — pin descriptor containing port, pin, mode, output type, speed, pull, alternate function, and base address
- `GPIO_Status` — status/error return codes for validation and pin operations
- `GPIO_PinState` — `GPIO_PIN_SET` / `GPIO_PIN_RESET`

Core concepts
-------------
A GPIO pin is modeled with a configuration object before it is programmed in hardware:

```c
typedef struct {
    GPIO_Port port;
    GPIO_Pin pin;
    GPIO_Mode mode;
    GPIO_OType otype;
    GPIO_OSpeed speed;
    GPIO_Pull pull;
    GPIO_AF af;
    uint32_t base;
} gpio_pin_cfg_t;
```

This makes each pin configuration explicit and keeps the register mapping close to the hardware model.

Main functions
--------------
- `GPIO_Status GPIO_pin_cfg_init(...)`
  - validates the requested pin configuration and stores the port base address

- `GPIO_Status GPIO_ConfigPin(const gpio_pin_cfg_t *cfg)`
  - enables the GPIO clock and applies pin mode, pull, output type, speed, and AF settings

- `GPIO_Status GPIO_WritePin(const gpio_pin_cfg_t *cfg, GPIO_PinState state)`
  - writes to the pin using the STM32 BSRR pattern for atomic set/reset operations

- `GPIO_Status GPIO_ReadPin(const gpio_pin_cfg_t *cfg, GPIO_PinState *out_state)`
  - reads the current input level for the selected pin

Usage example
-------------
```c
gpio_pin_cfg_t led = {
    .port  = GPIO_PORT_B,
    .pin   = PIN_13,
    .mode  = GPIO_MODE_OUTPUT,
    .otype = GPIO_PUSH_PULL,
    .speed = GPIO_SPEED_LOW,
    .pull  = GPIO_NOPULL,
    .af    = AF_0,
    .base  = GPIOB_BASE
};

if (GPIO_pin_cfg_init(&led, GPIO_PORT_B, PIN_13,
                      GPIO_MODE_OUTPUT,
                      GPIO_PUSH_PULL,
                      GPIO_SPEED_LOW,
                      GPIO_NOPULL,
                      AF_0) == GPIO_OK) {
    GPIO_ConfigPin(&led);
    GPIO_WritePin(&led, GPIO_PIN_SET);
}
```

Typical patterns
----------------

Input pin
```c
button.mode = GPIO_MODE_INPUT;
button.pull = GPIO_PULL_UP;
```

Output pin
```c
led.mode = GPIO_MODE_OUTPUT;
led.otype = GPIO_PUSH_PULL;
```

Alternate function pin
```c
uart_tx.mode = GPIO_MODE_AF;
uart_tx.af = AF_7;
```

Why this driver is useful
-------------------------
The GPIO driver is important because every embedded application begins with pin configuration. It is used for:

- LED control
- button input and interrupts
- UART TX/RX routing
- SPI selector and clock lines
- external peripheral enable signals

This is a foundational driver, but it is also a very good portfolio example because it demonstrates direct interaction with hardware and careful input validation.

Design trade-offs
-----------------
This implementation is intentionally explicit rather than heavily abstracted. That gives several advantages:

- direct mapping to the STM32 register model
- easier debugging on real hardware
- predictable behavior for higher-level drivers
- straightforward host-side unit testing

The trade-off is that the developer must understand the MCU pin mapping and alternate-function numbering. In bare-metal firmware, that is usually the correct trade-off because it keeps the code transparent and efficient.

Testing and CI
--------------
The GPIO driver is validated with host-side unit tests and is included in the repository CI flow. This makes it a good example of embedded software that is both minimally abstracted and still testable.

