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

/* Provide REG32 macro for host tests before any hardware headers use it */
#ifndef REG32
#define REG32(addr) (*mmio_ptr(addr))
#endif

#include "stm32_mmio.h"

/* Reset helper for tests */
void mmio_reset(void);

/* Host test stubs for RCC/NVIC side effects called by the drivers under
   test. These are no-ops on the fake mmio_map (real base addresses aren't
   remapped into it, unlike GPIOx_BASE above), since the point of these
   tests is the driver logic, not RCC/NVIC register behavior.
   Implemented in mmio_stub.c. */
#include "rcc.h"
#include "nvic.h"
void RCC_EnableAHB2(RCC_AHB2ENR_Pos pos);
void RCC_EnableAPB1(RCC_APB1ENR_Pos pos);
void RCC_EnableAPB2(RCC_APB2ENR_Pos pos);
void RCC_CCIPR_SelectClock(RCC_CCIPR_Field periph, RCC_ClockSource src);
uint32_t RCC_GetClockSourceFreq(RCC_ClockSource src);
void NVIC_EnableIRQ(IRQn_Type IRQn);
void NVIC_DisableIRQ(IRQn_Type IRQn);
void NVIC_SetPriority(IRQn_Type IRQn, IRQn_Priority priority);
void NVIC_ClearPendingIRQ(IRQn_Type IRQn);

#endif /* MMIO_STUB_H */