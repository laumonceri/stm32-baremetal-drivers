#ifndef STM32_GPIO_HW_H
#define STM32_GPIO_HW_H

#include <stdint.h>

/* Generic helpers */
#define BIT(n)                         (1UL << (n))

/* Generic register access */
#define REG32(addr)                    (*(volatile uint32_t *)(addr))

/* GPIO base addresses (STM32L4) */
#define GPIOA_BASE                     0x48000000UL
#define GPIOB_BASE                     0x48000400UL
#define GPIOC_BASE                     0x48000800UL
#define GPIOD_BASE                     0x48000C00UL
#define GPIOE_BASE                     0x48001000UL
#define GPIOF_BASE                     0x48001400UL
#define GPIOG_BASE                     0x48001800UL
#define GPIOH_BASE                     0x48001C00UL

/* GPIO register offsets */
#define GPIO_MODER_OFFSET              0x00U
#define GPIO_OTYPER_OFFSET             0x04U
#define GPIO_OSPEEDR_OFFSET            0x08U
#define GPIO_PUPDR_OFFSET              0x0CU
#define GPIO_IDR_OFFSET                0x10U
#define GPIO_ODR_OFFSET                0x14U

/* Atomic bit set/reset register */
#define GPIO_BSRR_OFFSET               0x18U

#define GPIO_AFRL_OFFSET               0x20U
#define GPIO_AFRH_OFFSET               0x24U

#define GPIO_MODER(base)               REG32((base) + GPIO_MODER_OFFSET)
#define GPIO_OTYPER(base)              REG32((base) + GPIO_OTYPER_OFFSET)
#define GPIO_OSPEEDR(base)             REG32((base) + GPIO_OSPEEDR_OFFSET)
#define GPIO_PUPDR(base)               REG32((base) + GPIO_PUPDR_OFFSET)
#define GPIO_IDR(base)                 REG32((base) + GPIO_IDR_OFFSET)
#define GPIO_ODR(base)                 REG32((base) + GPIO_ODR_OFFSET)
#define GPIO_AFRL(base)                REG32((base) + GPIO_AFRL_OFFSET)
#define GPIO_AFRH(base)                REG32((base) + GPIO_AFRH_OFFSET)

#endif