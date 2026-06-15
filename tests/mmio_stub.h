#ifndef MMIO_STUB_H
#define MMIO_STUB_H

#include <stdint.h>
#include <stddef.h>

/* Simple fake MMIO area for host tests - defined in mmio_stub.c */
#define MMIO_BASE 0x1000U
#define MMIO_SIZE 0x2000U
extern volatile uint32_t mmio_map[MMIO_SIZE / 4];

/* Map a fake address into mmio_map index (assumes aligned accesses) */
volatile uint32_t *mmio_ptr(uint32_t addr);

/* GPIO base addresses used by driver */
#define GPIOA_BASE  (MMIO_BASE + 0x000)
#define GPIOB_BASE  (MMIO_BASE + 0x100)
#define GPIOC_BASE  (MMIO_BASE + 0x200)
#define GPIOD_BASE  (MMIO_BASE + 0x300)
#define GPIOE_BASE  (MMIO_BASE + 0x400)
#define GPIOF_BASE  (MMIO_BASE + 0x500)
#define GPIOG_BASE  (MMIO_BASE + 0x600)
#define GPIOH_BASE  (MMIO_BASE + 0x700)

/* Provide REG32 macro for host tests to map addresses into mmio_map */
#ifndef REG32
#define REG32(addr) (*mmio_ptr(addr))
#endif

/* Reset helper for tests */
void mmio_reset(void);

/* Host test stub for RCC clock enable called by the driver.
   Implemented in mmio_stub.c. */
#include "rcc.h"
void RCC_EnableAHB2Clock(RCC_AHB2ENR_Pos pos);

#endif /* MMIO_STUB_H */