#ifndef STM32_I2C_HW_H
#define STM32_I2C_HW_H

#include "stm32_mmio.h"

/* Base addresses */
#define I2C1_BASE                      0x40005400UL

/* Register offsets */
#define I2C_CR1_OFFSET                 0x00U
#define I2C_CR2_OFFSET                 0x04U
#define I2C_TIMINGR_OFFSET             0x10U

#define I2C_ISR_OFFSET                 0x18U /* Interrupt and status register */
#define I2C_ICR_OFFSET                 0x1CU /* Interrupt clear register */
#define I2C_RXDR_OFFSET                0x24U /* 8-bit receive data register */
#define I2C_TXDR_OFFSET                0x28U /* 8-bit transmit data register */

/* Register access */
#define I2C_CR1(base)                  REG32((base) + I2C_CR1_OFFSET)
#define I2C_CR2(base)                  REG32((base) + I2C_CR2_OFFSET)
#define I2C_TIMINGR(base)              REG32((base) + I2C_TIMINGR_OFFSET)
#define I2C_ISR(base)                  REG32((base) + I2C_ISR_OFFSET)
#define I2C_ICR(base)                  REG32((base) + I2C_ICR_OFFSET)
#define I2C_RXDR(base)                 REG32((base) + I2C_RXDR_OFFSET)
#define I2C_TXDR(base)                 REG32((base) + I2C_TXDR_OFFSET)

/* CR1 bits */
#define I2C_CR1_PE                     BIT(0)
#define I2C_CR1_TXIE                   BIT(1)
#define I2C_CR1_RXIE                   BIT(2)

#define I2C_CR1_ADDRIE                 BIT(3)  /* Slave mode address match interrupt enable */
#define I2C_CR1_NACKIE                 BIT(4)  /* Not acknowledge received interrupt enable */
#define I2C_CR1_STOPIE                 BIT(5)  /* STOP detection interrupt enable */
#define I2C_CR1_TCIE                   BIT(6)  /* Transfer complete interrupt enable */

#define I2C_CR1_ERRIE                  BIT(7)

/* Digital noise filter bits [11:8] */
#define I2C_CR1_DNF_Pos                8U
#define I2C_CR1_DNF_Msk                (0xFU << I2C_CR1_DNF_Pos)
#define I2C_CR1_DNF(n)                 (((n) << I2C_CR1_DNF_Pos) & I2C_CR1_DNF_Msk)

#define I2C_CR1_ANFOFF                 BIT(12) /* Analog noise filter OFF */
#define I2C_CR1_TXDMAEN                BIT(14) /* DMA transmission requests enable */
#define I2C_CR1_RXDMAEN                BIT(15) /* DMA reception requests enable */
#define I2C_CR1_SBC                    BIT(16) /* Slave byte control */
#define I2C_CR1_NOSTRETCH              BIT(17) /* Clock stretching disable */

/* CR2 fields */

/* Slave address bits [9:0] */
#define I2C_CR2_SADD_Pos               0U
#define I2C_CR2_SADD_Msk               (0x3FFU << I2C_CR2_SADD_Pos)
#define I2C_CR2_SADD(n)                (((n) << I2C_CR2_SADD_Pos) & I2C_CR2_SADD_Msk)

#define I2C_CR2_RD_WRN                 BIT(10)
#define I2C_CR2_START                  BIT(13)
#define I2C_CR2_STOP                   BIT(14)
#define I2C_CR2_NACK                   BIT(15)

/* Number of bytes bits [23:16] */
#define I2C_CR2_NBYTES_Pos             16U
#define I2C_CR2_NBYTES_Msk             (0xFFU << I2C_CR2_NBYTES_Pos)
#define I2C_CR2_NBYTES(n)              (((n) << I2C_CR2_NBYTES_Pos) & I2C_CR2_NBYTES_Msk)

#define I2C_CR2_RELOAD                 BIT(24)
#define I2C_CR2_AUTOEND                BIT(25)

/* TIMINGR fields */

/* Timing prescaler bits [31:28] */
#define I2C_TIMINGR_PRESC_Pos          28U
#define I2C_TIMINGR_PRESC_Msk          (0xFU << I2C_TIMINGR_PRESC_Pos)
#define I2C_TIMINGR_PRESC(n)           (((n) << I2C_TIMINGR_PRESC_Pos) & I2C_TIMINGR_PRESC_Msk)

/* Data setup time bits [23:20] */
#define I2C_TIMINGR_SCLDEL_Pos         20U
#define I2C_TIMINGR_SCLDEL_Msk         (0xFU << I2C_TIMINGR_SCLDEL_Pos)
#define I2C_TIMINGR_SCLDEL(n)          (((n) << I2C_TIMINGR_SCLDEL_Pos) & I2C_TIMINGR_SCLDEL_Msk)

/* Data hold time bits [19:16] */
#define I2C_TIMINGR_SDADEL_Pos         16U
#define I2C_TIMINGR_SDADEL_Msk         (0xFU << I2C_TIMINGR_SDADEL_Pos)
#define I2C_TIMINGR_SDADEL(n)          (((n) << I2C_TIMINGR_SDADEL_Pos) & I2C_TIMINGR_SDADEL_Msk)

/* SCL high period bits [15:8] (master mode) */
#define I2C_TIMINGR_SCLH_Pos           8U
#define I2C_TIMINGR_SCLH_Msk           (0xFFU << I2C_TIMINGR_SCLH_Pos)
#define I2C_TIMINGR_SCLH(n)            (((n) << I2C_TIMINGR_SCLH_Pos) & I2C_TIMINGR_SCLH_Msk)

/* SCL low period bits [7:0] (master mode) */
#define I2C_TIMINGR_SCLL_Pos           0U
#define I2C_TIMINGR_SCLL_Msk           (0xFFU << I2C_TIMINGR_SCLL_Pos)
#define I2C_TIMINGR_SCLL(n)            (((n) << I2C_TIMINGR_SCLL_Pos) & I2C_TIMINGR_SCLL_Msk)

/* ISR bits */
#define I2C_ISR_TXE                    BIT(0)
#define I2C_ISR_TXIS                   BIT(1)
#define I2C_ISR_RXNE                   BIT(2)

#define I2C_ISR_NACKF                  BIT(4)
#define I2C_ISR_STOPF                  BIT(5)

#define I2C_ISR_TC                     BIT(6)  /* Transfer complete */

#define I2C_ISR_BUSY                   BIT(15)

/* ICR bits */
#define I2C_ICR_NACKCF                 BIT(4)  /* Not acknowledge flag clear */
#define I2C_ICR_STOPCF                 BIT(5)  /* STOP detection flag clear */

#endif