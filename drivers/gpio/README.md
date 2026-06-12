GPIO driver
===========

Small, portable GPIO driver for STM32 (stm32l4 family).

Overview
--------
This driver provides a simple, validated API to configure and use GPIO pins. It centralizes port->base mapping, input validation and MMIO access.

Header
------
See `drivers/gpio/include/gpio.h` for types and full prototypes.

Key types
---------
- `gpio_pin_cfg_t` — pin descriptor (port, pin, mode, otype, speed, pull, af, base)
- `GPIO_Status` — return codes (GPIO_OK, GPIO_ERROR_...)
- `GPIO_PinState` — `GPIO_PIN_SET` / `GPIO_PIN_RESET`

Main functions
--------------
- `GPIO_Status gpio_pin_cfg_init(gpio_pin_cfg_t *cfg, GPIO_Port port, GPIO_Pin pin, GPIO_Mode mode, GPIO_OType otype, GPIO_OSpeed speed, GPIO_Pull pull, GPIO_AF af)`
  - Validate parameters and cache port base in `cfg`.

- `GPIO_Status GPIO_ConfigPin(const gpio_pin_cfg_t *cfg)`
  - Enable port clock and apply mode/otype/pupd/speed/AF settings.

- `GPIO_Status GPIO_WritePin(const gpio_pin_cfg_t *cfg, GPIO_PinState state)`
  - Set or reset output using BSRR.

- `GPIO_Status GPIO_ReadPin(const gpio_pin_cfg_t *cfg, GPIO_PinState *out_state)`
  - Read input level into `out_state`.

Usage example
-------------
```c
gpio_pin_cfg_t cfg;
if (gpio_pin_cfg_init(&cfg, GPIO_PORT_B, PIN_13, GPIO_MODE_OUTPUT,
                     GPIO_PUSH_PULL, GPIO_SPEED_LOW, GPIO_NOPULL, AF_0) == GPIO_OK) {
    if (GPIO_ConfigPin(&cfg) == GPIO_OK) {
        GPIO_WritePin(&cfg, GPIO_PIN_SET);
    }
}
```

Notes & best practices
----------------------
- Always call `gpio_pin_cfg_init()` and check the return code before using `cfg` in other APIs.
- Public functions return `GPIO_Status` on error; check and handle errors in application code.
- The driver caches the port base in `cfg` for speed; do not manually mutate `cfg` fields after init.
- For unit testing, provide a host-side MMIO stub (map GPIO_* macros to memory) and run tests with a native toolchain. Use `cppcheck` and `clang-format` in CI.

