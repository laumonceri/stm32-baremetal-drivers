#ifndef I2C_DRIVER_H
#define I2C_DRIVER_H

#include "stm32_i2c_hw.h"
#include "rcc.h"
#include "gpio.h"

// I2C timing for:
// fI2CCLK = 8 MHz
// I2C = 100 kHz
// Analog filter ON
// Digital filter = 1
// TODO: This is hardcoded, change later
#define I2C_TIMING_100KHZ_8MHZ (  \
    I2C_TIMINGR_PRESC(0x1)  | \
    I2C_TIMINGR_SCLDEL(0x4) | \
    I2C_TIMINGR_SDADEL(0x2) | \
    I2C_TIMINGR_SCLH(0xF)   | \
    I2C_TIMINGR_SCLL(0x13)    \
)

// Pin
typedef struct {
    uint32_t base;
    uint32_t timing;
    gpio_pin_cfg_t scl;
    gpio_pin_cfg_t sda;
} i2c_cfg_t;

// rcc
typedef struct {
    RCC_APB1ENR1_Pos apb1;
    RCC_APB1RSTR1_Pos reset;
} rcc_i2c_clk_t;

// pin + rcc
typedef struct {
    rcc_i2c_clk_t clk;
    i2c_cfg_t i2c;
} i2c_device_t;


void i2c_init(const i2c_device_t *dev);

#endif