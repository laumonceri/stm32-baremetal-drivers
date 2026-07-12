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

/* UART base addresses */
#define LPUART1_BASE (MMIO_BASE + 0x800)
#define USART1_BASE  (MMIO_BASE + 0x900)
#define USART2_BASE  (MMIO_BASE + 0xA00)
#define USART3_BASE  (MMIO_BASE + 0xB00)

#define RCC_BASE     (MMIO_BASE + 0xC00)

/* Provide REG32 macro for host tests before any hardware headers use it */
#ifndef REG32
#define REG32(addr) (*mmio_ptr(addr))
#endif

#include "stm32_mmio.h"

/* Reset helper for tests */
void mmio_reset(void);

/* RCC_BASE now has its own fake slot above, so the real platform/src/rcc.c
   implementation is linked in and safe to test against directly.

   NVIC still doesn't have a fake base of its own, so its side effects are
   stubbed as no-ops here (implemented in mmio_stub.c) to avoid aliasing
   into RCC/UART's slots. */
#include "rcc.h"
#include "nvic.h"
void NVIC_EnableIRQ(IRQn_Type IRQn);
void NVIC_DisableIRQ(IRQn_Type IRQn);
void NVIC_SetPriority(IRQn_Type IRQn, IRQn_Priority priority);
void NVIC_ClearPendingIRQ(IRQn_Type IRQn);

#endif /* MMIO_STUB_H */