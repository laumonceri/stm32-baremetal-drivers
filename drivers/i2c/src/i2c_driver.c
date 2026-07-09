#include "i2c_driver.h"

static void i2c_clock_init(const i2c_device_t *dev) {
  // Enable peripheral clock
  RCC_EnableAPB1(dev->clk.apb1);
  // Select clock source TODO: change to come from dev->
  RCC_CCIPR_SelectClock(RCC_SEL_I2C1, RCC_CLK_PCLK);
}

static void i2c_gpio_init(const i2c_device_t *dev) {
  GPIO_ConfigPin(&dev->i2c.scl);
  GPIO_ConfigPin(&dev->i2c.sda);
}

void i2c_init(const i2c_device_t *dev) {
  // Enable clocks
  i2c_clock_init(dev);

  // reset peripheral
  RCC_ResetAPB1(dev->clk.reset);

  // GPIO configuratio - SCL/SDA
  i2c_gpio_init(dev);

  // Disable peripheral before configuration
  I2C_CR1(dev->i2c.base) &= ~I2C_CR1_PE;

  /* Configure filters */
  /* Enable analog filter */
  I2C_CR1(dev->i2c.base) &= ~I2C_CR1_ANFOFF;
  /* Digital noise filter = 1 cycle */
  I2C_CR1(dev->i2c.base) |= I2C_CR1_DNF(1);

  // Configure timing (depends on PCLK)
  I2C_TIMINGR(dev->i2c.base) = dev->i2c.timing;

  // Enable clock stretching
  I2C_CR1(dev->i2c.base) &= ~I2C_CR1_NOSTRETCH;

  // Enable I2C peripheral
  I2C_CR1(dev->i2c.base) |= I2C_CR1_PE;
}