#ifndef STM32_SPI_REGS_H
#define STM32_SPI_REGS_H

#include <stdint.h>
#include "stm32_mmio.h"

#define SPI_CR1_OFFSET    0x00U
#define SPI_CR2_OFFSET    0x04U
#define SPI_SR_OFFSET     0x08U
#define SPI_CRCPR_OFFSET  0x10U
#define SPI_RXCRCR_OFFSET 0x14U
#define SPI_DR_OFFSET     0x0CU

#define SPI_CR1(base)        REG32((base) + SPI_CR1_OFFSET)
#define SPI_CR2(base)        REG32((base) + SPI_CR2_OFFSET)
#define SPI_SR(base)         REG32((base) + SPI_SR_OFFSET)
#define SPI_CRCPR(base)      REG32((base) + SPI_CRCPR_OFFSET)
#define SPI_RXCRC(base)      REG32((base) + SPI_RXCRCR_OFFSET)
#define SPI_DR(base)         REG32((base) + SPI_DR_OFFSET)
/* Byte-width access to DR, required for 8-bit-or-smaller frames (FRXTH=1) -
   a 32-bit REG32 access desyncs the FIFO packing for DS <= 8-bit. */
#define SPI_DR8(base)        (*(volatile uint8_t *)((base) + SPI_DR_OFFSET))

/* CR1 */
#define SPI_CR1_CPHA        BIT(0)
#define SPI_CR1_CPOL        BIT(1)
#define SPI_CR1_MSTR        BIT(2)
/* BR[2:0] */
#define SPI_CR1_BR_Pos      3U
#define SPI_CR1_BR_Msk      (0x7U << SPI_CR1_BR_Pos)
#define SPI_CR1_BR(n)       (((n) << SPI_CR1_BR_Pos) & SPI_CR1_BR_Msk)
#define SPI_CR1_SPE         BIT(6)
#define SPI_CR1_LSBFIRST    BIT(7)
#define SPI_CR1_SSI         BIT(8)
#define SPI_CR1_SSM         BIT(9)
#define SPI_CR1_RXONLY      BIT(10)
#define SPI_CR1_CRCL        BIT(11)
#define SPI_CR1_CRCNEXT     BIT(12)
#define SPI_CR1_CRCEN       BIT(13)
#define SPI_CR1_BIDIOE      BIT(14)
#define SPI_CR1_BIDIMODE    BIT(15)

/* CR2 */
#define SPI_CR2_RXDMAEN     BIT(0)
#define SPI_CR2_TXDMAEN     BIT(1)
#define SPI_CR2_SSOE        BIT(2)
#define SPI_CR2_NSSP        BIT(3)
#define SPI_CR2_FRF         BIT(4)
#define SPI_CR2_ERRIE       BIT(5)
#define SPI_CR2_RXNEIE      BIT(6)
#define SPI_CR2_TXEIE       BIT(7)
/* DS[11:8] */
#define SPI_CR2_DS_Pos      8U
#define SPI_CR2_DS_Msk     (0xFU << SPI_CR2_DS_Pos)
#define SPI_CR2_DS(n)      (((n) << SPI_CR2_DS_Pos) & SPI_CR2_DS_Msk)
#define SPI_CR2_FRXTH      BIT(12)
#define SPI_CR2_LDMA_RX    BIT(13)
#define SPI_CR2_LDMA_TX    BIT(14)

/* SPI status register */
#define SPI_SR_RXNE        BIT(0)
#define SPI_SR_TXE         BIT(1)
#define SPI_SR_CRCERR      BIT(4)
#define SPI_SR_MODF        BIT(5)
#define SPI_SR_OVR         BIT(6)
#define SPI_SR_BSY         BIT(7)
#define SPI_SR_FRE         BIT(8)
/* FRLVL[10:9] */
#define SPI_SR_FRLVL_Pos   9U
#define SPI_SR_FRLVL_Msk   (0x3U << SPI_SR_FRLVL_Pos)
#define SPI_SR_FRLVL(n)    (((n) << SPI_SR_FRLVL_Pos) & SPI_SR_FRLVL_Msk)
/* FRLVL[12:11] */
#define SPI_SR_FTLVL_Pos   11U
#define SPI_SR_FTLVL_Msk   (0x3U << SPI_SR_FTLVL_Pos)
#define SPI_SR_FTLVL(n)    (((n) << SPI_SR_FTLVL_Pos) & SPI_SR_FTLVL_Msk)

#endif /* STM32_SPI_REGS_H */