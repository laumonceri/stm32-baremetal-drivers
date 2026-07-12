#ifndef STM32_GPIO_REGS_H
#define STM32_GPIO_REGS_H

/* GPIO register offsets (shared between production and tests) */
#define GPIO_MODER_OFFSET              0x00U
#define GPIO_OTYPER_OFFSET             0x04U
#define GPIO_OSPEEDR_OFFSET            0x08U
#define GPIO_PUPDR_OFFSET              0x0CU
#define GPIO_IDR_OFFSET                0x10U
#define GPIO_ODR_OFFSET                0x14U
#define GPIO_BSRR_OFFSET               0x18U /* Atomic bit set/reset register */
#define GPIO_AFRL_OFFSET               0x20U
#define GPIO_AFRH_OFFSET               0x24U

#define GPIO_MODER(base)               REG32((base) + GPIO_MODER_OFFSET)
#define GPIO_OTYPER(base)              REG32((base) + GPIO_OTYPER_OFFSET)
#define GPIO_OSPEEDR(base)             REG32((base) + GPIO_OSPEEDR_OFFSET)
#define GPIO_PUPDR(base)               REG32((base) + GPIO_PUPDR_OFFSET)
#define GPIO_IDR(base)                 REG32((base) + GPIO_IDR_OFFSET)
#define GPIO_ODR(base)                 REG32((base) + GPIO_ODR_OFFSET)
#define GPIO_BSRR(base)                REG32((base) + GPIO_BSRR_OFFSET)
#define GPIO_AFRL(base)                REG32((base) + GPIO_AFRL_OFFSET)
#define GPIO_AFRH(base)                REG32((base) + GPIO_AFRH_OFFSET)

#endif /* STM32_GPIO_REGS_H */