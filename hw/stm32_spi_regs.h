#ifndef STM32_SPI_REGS_H
#define STM32_SPI_REGS_H

#define SPI_CR1_OFFSET  0x00U
#define SPI_CR2_OFFSET  0x04U

#define SPI_CR1(spi) REG32((spi) + SPI_CR1_OFFSET)
#define SPI_CR2(spi)


#endif /* STM32_SPI_REGS_H */