#ifndef GPIO_H
#define GPIO_H

#include "stm32_gpio_hw.h"
#include "stm32_rcc_hw.h"
#include "rcc.h"
#include <stdint.h>
#include <stddef.h>

typedef enum {
    GPIO_PORT_A = 0,
    GPIO_PORT_B,
    GPIO_PORT_C,
    GPIO_PORT_D,
    GPIO_PORT_E,
    GPIO_PORT_F,
    GPIO_PORT_G,
    GPIO_PORT_H
} GPIO_Port;

typedef enum {
    PIN_0 = 0,
    PIN_1,
    PIN_2,
    PIN_3,
    PIN_4,
    PIN_5,
    PIN_6,
    PIN_7,
    PIN_8,
    PIN_9,
    PIN_10,
    PIN_11,
    PIN_12,
    PIN_13,
    PIN_14,
    PIN_15
} GPIO_Pin;

typedef enum {
    GPIO_MODE_INPUT  = 0b00,
    GPIO_MODE_OUTPUT = 0b01,
    GPIO_MODE_AF     = 0b10,
    GPIO_MODE_ANALOG = 0b11
} GPIO_Mode;

typedef enum {
    GPIO_PUSH_PULL = 0,
    GPIO_OPEN_DRAIN
} GPIO_OType;

typedef enum {
    GPIO_SPEED_LOW = 0b00, 
    GPIO_SPEED_MEDIUM = 0b01,
    GPIO_SPEED_HIGH = 0b10, 
    GPIO_SPEED_VERY_HIGH = 0b11
} GPIO_OSpeed;

typedef enum {
    GPIO_NOPULL = 0b00,
    GPIO_PULL_UP = 0b01,
    GPIO_PULL_DOWN = 0b10
} GPIO_Pull;

typedef enum {
    AF_0 = 0,
    AF_1,
    AF_2,
    AF_3,
    AF_4,
    AF_5,
    AF_6,
    AF_7,
    AF_8,
    AF_9,
    AF_10,
    AF_11,
    AF_12,
    AF_13,
    AF_14,
    AF_15
} GPIO_AF;

typedef enum {
    GPIO_PIN_RESET = 0,
    GPIO_PIN_SET
} GPIO_PinState;

// TODO: replace this for gpio_pin_cfg_t
typedef struct {
    GPIO_Port port;
    uint32_t pin;
} Pin_Config;

typedef struct {
    GPIO_Port port;
    GPIO_Pin pin;
    GPIO_Mode mode;
    GPIO_OType otype;
    GPIO_OSpeed speed;
    GPIO_Pull pull;
    GPIO_AF af;
} gpio_pin_cfg_t;


void GPIO_ConfigPin(const gpio_pin_cfg_t *cfg);

/**
 * @brief Initialize a GPIO pin with a specific mode
 * 
 * This function enables the peripheral clock for the GPIO port,
 * then configures the pin mode (input, output, alternate function, analog).
 * 
 * @param port GPIO port to configure (A–E)
 * @param pin Pin number (0–15)
 * @param mode Pin mode (GPIO_MODE_INPUT, GPIO_MODE_OUTPUT, etc.)
 */
void GPIO_Init(GPIO_Port port, GPIO_Pin pin, GPIO_Mode mode, GPIO_Pull pull);

/**
 * @brief Set or reset a GPIO pin
 * 
 * Writes a logic high or low to the output data register for the specified pin.
 * The pin must be configured as output.
 * 
 * @param port GPIO port of the pin
 * @param pin Pin number (0–15)
 * @param state GPIO_PIN_SET to drive high, GPIO_PIN_RESET to drive low
 */
void GPIO_WritePin(GPIO_Port port, GPIO_Pin pin, GPIO_PinState state);

/**
 * @brief Read the current state of a GPIO pin
 * 
 * Reads the input data register for the specified pin and returns its logic level.
 * 
 * @param port GPIO port of the pin
 * @param pin Pin number (0–15)
 * @return GPIO_PIN_SET if high, GPIO_PIN_RESET if low
 */
GPIO_PinState GPIO_ReadPin(GPIO_Port port, GPIO_Pin pin);

#endif