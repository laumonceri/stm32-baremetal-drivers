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

typedef enum {
    GPIO_OK = 0,
    GPIO_ERROR_INVALID_PORT = 1,
    GPIO_ERROR_INVALID_PIN = 2,
    GPIO_ERROR_INVALID_MODE = 3,
    GPIO_ERROR_INVALID_AF = 4,
    GPIO_ERROR_NULL_CONFIG = 5,
    GPIO_ERROR_INVALID_STATE = 6
} GPIO_Status;

GPIO_Status GPIO_pin_cfg_init(gpio_pin_cfg_t *cfg,
                              GPIO_Port port,
                              GPIO_Pin pin,
                              GPIO_Mode mode,
                              GPIO_OType otype,
                              GPIO_OSpeed speed,
                              GPIO_Pull pull,
                              GPIO_AF af);

/**
 * @brief Initialize a GPIO pin with a specific mode
 * 
 * This function enables the peripheral clock for the GPIO port,
 * then configures the pin mode (input, output, alternate function, analog).
 * 
 * @param cfg Pointer to an initialized gpio_pin_cfg_t
 */
GPIO_Status GPIO_ConfigPin(const gpio_pin_cfg_t *cfg);

/**
 * @brief Set or reset a GPIO pin
 * 
 * Writes a logic high or low to the output data register for the specified pin.
 * The pin must be configured as output.
 * 
 * @param cfg Pointer to an initialized gpio_pin_cfg_t
 * @param state GPIO_PIN_SET to drive high, GPIO_PIN_RESET to drive low
 */
GPIO_Status GPIO_WritePin(const gpio_pin_cfg_t *cfg, GPIO_PinState state);

/**
 * @brief Read the current state of a GPIO pin
 * 
 * Reads the input data register for the specified pin and returns the logic level
 * via out_state. Returns error code on invalid parameters.
 * 
 * @param cfg Pointer to an initialized gpio_pin_cfg_t
 * @param out_state Pointer to receive pin state
 * @return GPIO_OK on success or an error code
 */
GPIO_Status GPIO_ReadPin(const gpio_pin_cfg_t *cfg, GPIO_PinState *out_state);

#endif