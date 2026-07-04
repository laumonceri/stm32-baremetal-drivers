#ifndef STM32_UART_HW_H
#define STM32_UART_HW_H

#include "stm32_mmio.h"

#define LPUART1_BASE   0x40008000UL
#define USART1_BASE    0x40013800UL
#define USART2_BASE    0x40004400UL

/* Register offsets */
#define UART_CR1_OFFSET    0x00U
#define UART_CR2_OFFSET    0x04U
#define UART_BRR_OFFSET    0x0CU
#define UART_ISR_OFFSET    0x1CU
#define UART_RDR_OFFSET    0x24U
#define UART_TDR_OFFSET    0x28U
#define UART_ICR_OFFSET    0x20U

/* Register access */
#define UART_CR1(base)     REG32((base) + UART_CR1_OFFSET)
#define UART_CR2(base)     REG32((base) + UART_CR2_OFFSET)
#define UART_BRR(base)     REG32((base) + UART_BRR_OFFSET)
#define UART_ISR(base)     REG32((base) + UART_ISR_OFFSET)
#define UART_RDR(base)     REG32((base) + UART_RDR_OFFSET)
#define UART_TDR(base)     REG32((base) + UART_TDR_OFFSET)
#define UART_ICR(base)     REG32((base) + UART_ICR_OFFSET)

/* CR1 bits */
#define UART_CR1_UE        BIT(0)
#define UART_CR1_RE        BIT(2)
#define UART_CR1_TE        BIT(3)
#define UART_CR1_RXNEIE    BIT(5)
#define UART_CR1_TCIE      BIT(6)
#define UART_CR1_TXEIE     BIT(7)

/* ISR bits */
#define UART_ISR_RXNE      BIT(5)
#define UART_ISR_TC        BIT(6)
#define UART_ISR_TXE       BIT(7)

#endif