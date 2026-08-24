# GPIO Driver

## Overview

The GPIO driver provides the low-level pin abstraction used across the STM32 bare-metal project. It configures pin mode, output type, pull-up/down behavior, speed, and alternate-function mapping without relying on HAL or CMSIS.

This is the foundation layer for the rest of the firmware: LEDs, buttons, UART lines, SPI chip-selects, and peripheral enable pins all ultimately depend on this driver.

## Why this driver matters

GPIO is the first hardware primitive in any embedded system. If the pin configuration is wrong, the rest of the driver stack becomes unreliable. This driver keeps the logic simple, direct, and easy to validate.

## Driver API

The public interface is defined in `drivers/gpio/include/gpio.h`.

### Core configuration type

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

### Main functions

```c
GPIO_Status GPIO_pin_cfg_init(gpio_pin_cfg_t *cfg,
                              GPIO_Port port,
                              GPIO_Pin pin,
                              GPIO_Mode mode,
                              GPIO_OType otype,
                              GPIO_OSpeed speed,
                              GPIO_Pull pull,
                              GPIO_AF af);

GPIO_Status GPIO_ConfigPin(const gpio_pin_cfg_t *cfg);
GPIO_Status GPIO_WritePin(const gpio_pin_cfg_t *cfg, GPIO_PinState state);
GPIO_Status GPIO_ReadPin(const gpio_pin_cfg_t *cfg, GPIO_PinState *out_state);
```

## Typical use cases

### Output pin

Used for LEDs, enable pins, and other digital actuators.

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

GPIO_pin_cfg_init(&led, GPIO_PORT_B, PIN_13,
                  GPIO_MODE_OUTPUT,
                  GPIO_PUSH_PULL,
                  GPIO_SPEED_LOW,
                  GPIO_NOPULL,
                  AF_0);

GPIO_ConfigPin(&led);
GPIO_WritePin(&led, GPIO_PIN_SET);
```

### Input pin

Used for buttons and digital sensors.

```c
button.mode = GPIO_MODE_INPUT;
button.pull = GPIO_PULL_UP;
```

### Alternate function pin

Used for UART TX/RX and SPI MOSI/MISO/SCK pins.

```c
uart_tx.mode = GPIO_MODE_AF;
uart_tx.af = AF_7;
```

## How it works

The driver configures the relevant GPIO MODER, OTYPER, OSPEEDR, PUPDR, AFR, and BSRR hardware registers for the selected port and pin. It also validates inputs before writing to hardware so bad arguments do not silently misconfigure the MCU.

## Design trade-offs

The driver intentionally favors explicit register-driven logic over heavy abstraction:

- straightforward mapping to STM32 hardware behavior
- easier debugging and validation
- predictable control of output pins and alternate functions
- simple integration with higher-level drivers

The trade-off is that the developer must understand STM32 pin numbering and alternate function mapping. For bare-metal firmware, that transparency is usually the right choice.

## Testing and validation

The GPIO driver is validated with host-side unit tests and is included in the repository CI path.
