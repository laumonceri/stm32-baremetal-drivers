#include "gpio.h"

/* Convert port enum to base address */
uint32_t GPIO_GetBaseAddress(GPIO_Port port)
{
    switch (port)
    {
        case GPIO_PORT_A: return GPIOA_BASE;
        case GPIO_PORT_B: return GPIOB_BASE;
        case GPIO_PORT_C: return GPIOC_BASE;
        case GPIO_PORT_D: return GPIOD_BASE;
        case GPIO_PORT_E: return GPIOE_BASE;
        case GPIO_PORT_F: return GPIOF_BASE;
        case GPIO_PORT_G: return GPIOG_BASE;
        case GPIO_PORT_H: return GPIOH_BASE;
        default: return 0;
    }
}

static void GPIO_setMode(uint32_t base, GPIO_Pin pin, GPIO_Mode mode)
{
    GPIO_MODER(base) &= ~(0x3U << (pin * 2U));
    GPIO_MODER(base) |=  ((mode & 0x3U) << (pin * 2U));
}

/* Port output type register */
static void GPIO_setOTYPE(uint32_t base, GPIO_Pin pin, GPIO_OType otype)
{
    GPIO_OTYPER(base) &= ~(1U << pin);
    GPIO_OTYPER(base) |=  ((otype & 1U) << pin);
}

/* Output speed register */
static void GPIO_setOSPEED(uint32_t base, GPIO_Pin pin, GPIO_OSpeed ospeed)
{
    GPIO_OSPEEDR(base) &= ~(0x3U << (pin * 2U));
    GPIO_OSPEEDR(base) |=  ((ospeed & 0x3U) << (pin * 2U));
}

/* Pull-up / pull-down register */
static void GPIO_setPUPD(uint32_t base, GPIO_Pin pin, GPIO_Pull pull)
{
    GPIO_PUPDR(base) &= ~(0x3U << (pin * 2U));
    GPIO_PUPDR(base) |=  ((pull & 0x3U) << (pin * 2U));
}

/* Alternate function register */
static void GPIO_setAF(uint32_t base, GPIO_Pin pin, GPIO_AF af)
{
    if (pin < 8)
    {
        GPIO_AFRL(base) &= ~(0xFU << (pin * 4U));
        GPIO_AFRL(base) |=  ((af & 0xFU) << (pin * 4U));
    }
    else
    {
        pin -= 8;

        GPIO_AFRH(base) &= ~(0xFU << (pin * 4U));
        GPIO_AFRH(base) |=  ((af & 0xFU) << (pin * 4U));
    }
}

void GPIO_ConfigPin(const gpio_pin_cfg_t *cfg)
{
    uint32_t base = GPIO_GetBaseAddress(cfg->port);

    if (base == 0U)
        return;
    // this only works because RCC_AHB2ENR_Pos and GPIO_Port
    // are mapped equally for GPIO
   RCC_EnableAHB2Clock(cfg->port);

    GPIO_setMode(base, cfg->pin, cfg->mode);
    GPIO_setOTYPE(base, cfg->pin, cfg->otype);
    GPIO_setPUPD(base, cfg->pin, cfg->pull);
    GPIO_setOSPEED(base, cfg->pin, cfg->speed);

    if (cfg->mode == GPIO_MODE_AF)
    {
        GPIO_setAF(base, cfg->pin, cfg->af);
    }
}

void GPIO_Init(GPIO_Port port,
               GPIO_Pin pin,
               GPIO_Mode mode,
               GPIO_Pull pull)
{
    uint32_t base = GPIO_GetBaseAddress(port);

    if (base == 0U || pin > PIN_15)
        return;

    RCC_EnableAHB2Clock(port);

    GPIO_setMode(base, pin, mode);
    GPIO_setPUPD(base, pin, pull);
}

void GPIO_WritePin(GPIO_Port port,
                   GPIO_Pin pin,
                   GPIO_PinState state)
{
    uint32_t base = GPIO_GetBaseAddress(port);

    if (base == 0U || pin > PIN_15)
        return;

    if (state == GPIO_PIN_SET)
    {
        REG32(base + GPIO_BSRR_OFFSET) = (1U << pin);
    }
    else
    {
        REG32(base + GPIO_BSRR_OFFSET) = (1U << (pin + 16U));
    }
}

GPIO_PinState GPIO_ReadPin(GPIO_Port port,
                           GPIO_Pin pin)
{
    uint32_t base = GPIO_GetBaseAddress(port);

    if (base == 0U || pin > PIN_15)
        return GPIO_PIN_RESET;

    return (GPIO_IDR(base) & (1U << pin))
           ? GPIO_PIN_SET
           : GPIO_PIN_RESET;
}