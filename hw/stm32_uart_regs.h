#ifndef SMT32_UART_REGS_H
#define SMT32_UART_REGS_H

/* Register offsets */
#define UART_CR1_OFFSET    0x00U
#define UART_CR2_OFFSET    0x04U
#define UART_CR3_OFFSET    0x08U
#define UART_BRR_OFFSET    0x0CU
#define UART_ISR_OFFSET    0x1CU // interrupt and status register
#define UART_ICR_OFFSET    0x20U // interrupt flag clear register
#define UART_RDR_OFFSET    0x24U // receive data register
#define UART_TDR_OFFSET    0x28U // transmit data register

/* Register access */
#define UART_CR1(base)     REG32((base) + UART_CR1_OFFSET)
#define UART_CR2(base)     REG32((base) + UART_CR2_OFFSET)
#define UART_CR3(base)     REG32((base) + UART_CR3_OFFSET)
#define UART_BRR(base)     REG32((base) + UART_BRR_OFFSET)
#define UART_ISR(base)     REG32((base) + UART_ISR_OFFSET)
#define UART_ICR(base)     REG32((base) + UART_ICR_OFFSET)
#define UART_RDR(base)     REG32((base) + UART_RDR_OFFSET)
#define UART_TDR(base)     REG32((base) + UART_TDR_OFFSET)


/* CR1 bits */
#define UART_CR1_UE        BIT(0)
#define UART_CR1_RE        BIT(2)
#define UART_CR1_TE        BIT(3)
#define UART_CR1_RXNEIE    BIT(5)
#define UART_CR1_TCIE      BIT(6)
#define UART_CR1_TXEIE     BIT(7)
#define UART_CR1_PS        BIT(9)
#define UART_CR1_PCE       BIT(10)
#define UART_CR1_M0        BIT(12)
#define UART_CR1_M1        BIT(28)

#define UART_CR2_STOP_Pos      12U
#define UART_CR2_STOP_Msk      (0x3U << UART_CR2_STOP_Pos)
#define UART_CR2_STOP(n)       (((n) << UART_CR2_STOP_Pos) & UART_CR2_STOP_Msk)

#define UART_STOP_1_BIT        (0x0U << UART_CR2_STOP_Pos)
#define UART_STOP_0_5_BIT      (0x1U << UART_CR2_STOP_Pos)
#define UART_STOP_2_BIT        (0x2U << UART_CR2_STOP_Pos)
#define UART_STOP_1_5_BIT      (0x3U << UART_CR2_STOP_Pos)

/* ISR bits */
#define UART_ISR_PE        BIT(0)
#define UART_ISR_ORE       BIT(3)
#define UART_ISR_RXNE      BIT(5)
#define UART_ISR_TC        BIT(6)
#define UART_ISR_TXE       BIT(7)

/* ICR bits */
#define UART_ICR_PECF      BIT(0)
#define UART_ICR_ORECF     BIT(3)

#endif /* SMT32_UART_REGS_H */