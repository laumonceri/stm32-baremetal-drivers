#if defined(UNIT_TEST_HOST)
#include "../../../tests/mmio_stub.h"
#endif

#include "gpio.h"

/* Convert port enum to base address */
uint32_t GPIO_GetBaseAddress(GPIO_Port port) {
  switch (port) {
  case GPIO_PORT_A:
    return GPIOA_BASE;
  case GPIO_PORT_B:
    return GPIOB_BASE;
  case GPIO_PORT_C:
    return GPIOC_BASE;
  case GPIO_PORT_D:
    return GPIOD_BASE;
  case GPIO_PORT_E:
    return GPIOE_BASE;
  case GPIO_PORT_F:
    return GPIOF_BASE;
  case GPIO_PORT_G:
    return GPIOG_BASE;
  case GPIO_PORT_H:
    return GPIOH_BASE;
  default:
    return 0;
  }
}

/* Validate cfg and return resolved base via out_base */
static GPIO_Status gpio_validate_and_get_base(const gpio_pin_cfg_t *cfg,
                                              uint32_t *out_base) {
  if (cfg == NULL) {
    return GPIO_ERROR_NULL_CONFIG;
  }
  if (cfg->port < GPIO_PORT_A || cfg->port > GPIO_PORT_H) {
    return GPIO_ERROR_INVALID_PORT;
  }
  if (cfg->pin > PIN_15) {
    return GPIO_ERROR_INVALID_PIN;
  }
  if (cfg->mode == GPIO_MODE_AF && cfg->af > AF_15) {
    return GPIO_ERROR_INVALID_AF;
  }
  uint32_t base = cfg->base ? cfg->base : GPIO_GetBaseAddress(cfg->port);
  if (base == 0U) {
    return GPIO_ERROR_INVALID_PORT;
  }
  if (out_base)
    *out_base = base;

  return GPIO_OK;
}

GPIO_Status GPIO_pin_cfg_init(gpio_pin_cfg_t *cfg, GPIO_Port port, GPIO_Pin pin,
                              GPIO_Mode mode, GPIO_OType otype,
                              GPIO_OSpeed speed, GPIO_Pull pull, GPIO_AF af) {
  if (cfg == NULL) {
    return GPIO_ERROR_NULL_CONFIG;
  }

  /* validate inputs before writing to cfg */
  if (port < GPIO_PORT_A || port > GPIO_PORT_H) {
    return GPIO_ERROR_INVALID_PORT;
  }
  if (pin < PIN_0 || pin > PIN_15) {
    return GPIO_ERROR_INVALID_PIN;
  }
  if (mode == GPIO_MODE_AF && af > AF_15) {
    return GPIO_ERROR_INVALID_AF;
  }

  /* compute and cache base address */
  cfg->base = GPIO_GetBaseAddress(port);
  if (cfg->base == 0U) {
    return GPIO_ERROR_INVALID_PORT;
  }

  cfg->port = port;
  cfg->pin = pin;
  cfg->mode = mode;
  cfg->otype = otype;
  cfg->speed = speed;
  cfg->pull = pull;
  cfg->af = af;

  return GPIO_OK;
}

/* Static low-level register helpers that accept resolved base and pin */
static void GPIO_setMode(uint32_t base, GPIO_Pin pin, GPIO_Mode mode) {
  GPIO_MODER(base) &= ~(0x3U << (pin * 2U));
  GPIO_MODER(base) |= ((mode & 0x3U) << (pin * 2U));
}

static void GPIO_setOTYPE(uint32_t base, GPIO_Pin pin, GPIO_OType otype) {
  GPIO_OTYPER(base) &= ~(1U << pin);
  GPIO_OTYPER(base) |= ((otype & 1U) << pin);
}

static void GPIO_setOSPEED(uint32_t base, GPIO_Pin pin, GPIO_OSpeed ospeed) {
  GPIO_OSPEEDR(base) &= ~(0x3U << (pin * 2U));
  GPIO_OSPEEDR(base) |= ((ospeed & 0x3U) << (pin * 2U));
}

static void GPIO_setPUPD(uint32_t base, GPIO_Pin pin, GPIO_Pull pull) {
  GPIO_PUPDR(base) &= ~(0x3U << (pin * 2U));
  GPIO_PUPDR(base) |= ((pull & 0x3U) << (pin * 2U));
}

static void GPIO_setAF(uint32_t base, GPIO_Pin pin, GPIO_AF af) {
  uint32_t pin_idx = (uint32_t)pin;
  if (pin_idx < 8) {
    GPIO_AFRL(base) &= ~(0xFU << (pin_idx * 4U));
    GPIO_AFRL(base) |= ((af & 0xFU) << (pin_idx * 4U));
  } else {
    pin_idx -= 8;
    GPIO_AFRH(base) &= ~(0xFU << (pin_idx * 4U));
    GPIO_AFRH(base) |= ((af & 0xFU) << (pin_idx * 4U));
  }
}

GPIO_Status GPIO_ConfigPin(const gpio_pin_cfg_t *cfg) {
  uint32_t base;
  GPIO_Status st = gpio_validate_and_get_base(cfg, &base);
  if (st != GPIO_OK) {
    return st;
  }

  /* Enable clock for the port */
  RCC_EnableAHB2((RCC_AHB2ENR_Pos)cfg->port);

  /* Select the alternate function before switching MODER into AF mode, so
   * the pin is never briefly AF-driven by the wrong peripheral (AF0) while
   * MODER and AFRL/AFRH are set in separate writes. */
  if (cfg->mode == GPIO_MODE_AF) {
    GPIO_setAF(base, cfg->pin, cfg->af);
  }

  GPIO_setOTYPE(base, cfg->pin, cfg->otype);
  GPIO_setPUPD(base, cfg->pin, cfg->pull);
  GPIO_setOSPEED(base, cfg->pin, cfg->speed);
  GPIO_setMode(base, cfg->pin, cfg->mode);

  return GPIO_OK;
}

GPIO_Status GPIO_WritePin(const gpio_pin_cfg_t *cfg, GPIO_PinState state) {
  uint32_t base;
  GPIO_Status st = gpio_validate_and_get_base(cfg, &base);
  if (st != GPIO_OK) {
    return st;
  }

  if (state == GPIO_PIN_SET) {
    REG32(base + GPIO_BSRR_OFFSET) = (1U << cfg->pin);
  } else {
    REG32(base + GPIO_BSRR_OFFSET) = (1U << (cfg->pin + 16U));
  }

  return GPIO_OK;
}

GPIO_Status GPIO_ReadPin(const gpio_pin_cfg_t *cfg, GPIO_PinState *out_state) {
  uint32_t base;
  GPIO_Status st = gpio_validate_and_get_base(cfg, &base);
  if (st != GPIO_OK) {
    return st;
  }

  if (out_state == NULL)
    return GPIO_ERROR_NULL_CONFIG;

  *out_state =
      (GPIO_IDR(base) & (1U << cfg->pin)) ? GPIO_PIN_SET : GPIO_PIN_RESET;
  return GPIO_OK;
}