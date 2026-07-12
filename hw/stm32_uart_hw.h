#ifndef STM32_UART_HW_H
#define STM32_UART_HW_H

#include "stm32_mmio.h"

#ifndef LPUART1_BASE
#define LPUART1_BASE   0x40008000UL
#endif
#ifndef USART1_BASE
#define USART1_BASE    0x40013800UL
#endif
#ifndef USART2_BASE
#define USART2_BASE    0x40004400UL
#endif
#ifndef USART3_BASE
#define USART3_BASE    0x40004800UL
#endif

#include "stm32_uart_regs.h"

#endif /* STM32_UART_HW_H */
