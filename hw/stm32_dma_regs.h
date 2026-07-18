#ifndef STM32_DMA_REGS_H
#define STM32_DMA_REGS_H

#include "stm32_mmio.h"

#define DMA_CHANNEL_OFFSET 0x14U

#define DMA_ISR_OFFSET      0x00U // Interrupt status
#define DMA_IFCR_OFFSET     0x04U // Interrupt flag clear
#define DMA_CCR_OFFSET      0x08U // Channel x configuration -> 0x08 + 0x14*( x - 1), (x = 1 to 7)
#define DMA_CNDTR_OFFSET    0x0C // Channel x number of data to transfer -> 0x0C + 0x14 * (x - 1), (x = 1 to 7)
#define DMA_CPAR_OFFSET     0x10U // Channel x peripheral address -> 0x10 + 0x14 * (x - 1), (x = 1 to 7)
#define DMA_CMAR_OFFSET     0x14U // Channel x memory address -> 0x14 + 0x14 * (x - 1), (x = 1 to 7)
#define DMA_CSELR_OFFSET    0xA8U // Channel selection


#define DMA_ISR(base)            REG32((base) + DMA_ISR_OFFSET)
#define DMA_IFCR(base)           REG32((base) + DMA_IFCR_OFFSET)
#define DMA_CCR(base, channel)   REG32((base) + DMA_CCR_OFFSET + (DMA_CHANNEL_OFFSET * ((channel) - 1)))
#define DMA_CNDTR(base, channel) REG32((base) + DMA_CNDTR_OFFSET + (DMA_CHANNEL_OFFSET * ((channel) - 1)))
#define DMA_CPAR(base, channel)  REG32((base) + DMA_CPAR_OFFSET + (DMA_CHANNEL_OFFSET * ((channel) - 1)))
#define DMA_CMAR(base, channel)  REG32((base) + DMA_CMAR_OFFSET + (DMA_CHANNEL_OFFSET * ((channel) - 1)))
#define DMA_CSELR(base)          REG32((base) + DMA_CSELR_OFFSET)

#define DMA_CSELR_Pos(channel)         (((channel) - 1) * 4U)
#define DMA_CSELR_Msk(channel)         (0xFUL << DMA_CSELR_Pos(channel))
#define DMA_CSELR_SEL(channel, request) (((request) << DMA_CSELR_Pos(channel)) & DMA_CSELR_Msk(channel))

#define DMA_CCR_EN      BIT(0)
#define DMA_CCR_TCIE    BIT(1) // Transfer complete interrupt enable
#define DMA_CCR_HTIE    BIT(2) // Half transfer interrupt enable
#define DMA_CCR_TEIE    BIT(3) // Transfer error interrupt enable
#define DMA_CCR_DIR     BIT(4) // Data transfer direction
#define DMA_CCR_CIRC    BIT(5) // Circular mode
#define DMA_CCR_PINC    BIT(6) // Peripheral increment mode
#define DMA_CCR_MINC    BIT(7) // Memory increment mode
/* PSIZE[9:8] */
#define DMA_CCR_PSIZE_Pos   8U
#define DMA_CCR_PSIZE_Msk   (0x3UL << DMA_CCR_PSIZE_Pos)
#define DMA_CCR_PSIZE(n)    (((n) << DMA_CCR_PSIZE_Pos) & DMA_CCR_PSIZE_Msk)
/* MSIZE[11:10] */
#define DMA_CCR_MSIZE_Pos   10U
#define DMA_CCR_MSIZE_Msk   (0x3UL << DMA_CCR_MSIZE_Pos)
#define DMA_CCR_MSIZE(n)    (((n) << DMA_CCR_MSIZE_Pos) & DMA_CCR_MSIZE_Msk)
/* PL[13:12] -> Priority level */
#define DMA_CCR_PL_Pos      12U
#define DMA_CCR_PL_Msk      (0x3UL << DMA_CCR_PL_Pos)
#define DMA_CCR_PL(n)       (((n) << DMA_CCR_PL_Pos) & DMA_CCR_PL_Msk)
#define DMA_CCR_MEM2MEM     BIT(14)

#endif /* STM32_DMA_REGS_H */
