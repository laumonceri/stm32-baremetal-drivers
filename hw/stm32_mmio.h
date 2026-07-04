#ifndef STM32_MMIO_H
#define STM32_MMIO_H

#ifndef REG32
#define REG32(addr) (*(volatile uint32_t *)(addr))
#endif

#endif /* STM32_MMIO_H */