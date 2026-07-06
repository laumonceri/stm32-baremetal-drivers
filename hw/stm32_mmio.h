#ifndef STM32_MMIO_H
#define STM32_MMIO_H

#ifndef REG32
#define REG32(addr) (*(volatile uint32_t *)(addr))
#endif

#define BIT(n)                         (1UL << (n))

#endif /* STM32_MMIO_H */