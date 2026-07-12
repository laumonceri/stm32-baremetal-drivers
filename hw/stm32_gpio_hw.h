#ifndef STM32_GPIO_HW_H
#define STM32_GPIO_HW_H

//#include <stdint.h>

#ifndef GPIOA_BASE
#define GPIOA_BASE                     0x48000000UL
#endif
#ifndef GPIOB_BASE
#define GPIOB_BASE                     0x48000400UL
#endif
#ifndef GPIOC_BASE
#define GPIOC_BASE                     0x48000800UL
#endif
#ifndef GPIOD_BASE
#define GPIOD_BASE                     0x48000C00UL
#endif
#ifndef GPIOE_BASE
#define GPIOE_BASE                     0x48001000UL
#endif
#ifndef GPIOF_BASE
#define GPIOF_BASE                     0x48001400UL
#endif
#ifndef GPIOG_BASE
#define GPIOG_BASE                     0x48001800UL
#endif
#ifndef GPIOH_BASE
#define GPIOH_BASE                     0x48001C00UL
#endif

#include "stm32_gpio_regs.h"

#endif /* STM32_GPIO_HW_H */