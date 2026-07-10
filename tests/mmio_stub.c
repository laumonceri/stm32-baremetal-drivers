#include "mmio_stub.h"
#include <stdio.h>
#include <stdlib.h>

volatile uint32_t mmio_map[MMIO_SIZE / 4];

volatile uint32_t *mmio_ptr(uint32_t addr) {
    const size_t num_entries = sizeof(mmio_map) / sizeof(mmio_map[0]);
    if (addr < MMIO_BASE) {
        /* map below-base addresses to index 0 */
        return &mmio_map[0];
    }
    size_t idx = (size_t)((addr - MMIO_BASE) / 4U);
    if (idx >= num_entries) {
        /* clamp out-of-range addresses to the last entry to avoid crashes */
        idx = num_entries - 1;
    }
    return &mmio_map[idx];
}

void mmio_reset(void) {
    for (size_t i = 0u; i < sizeof(mmio_map)/4; ++i) {
        mmio_map[i] = 0;
    }
}

void RCC_EnableAHB2(RCC_AHB2ENR_Pos pos) {
    (void)pos; // no-op for tests
}

void RCC_EnableAPB1(RCC_APB1ENR_Pos pos) {
    (void)pos;  
}

void RCC_EnableAPB2(RCC_APB2ENR_Pos pos) {
    (void)pos;  
}

void RCC_CCIPR_SelectClock(RCC_CCIPR_Field periph, RCC_ClockSource src) {
    (void)periph;
    (void)src;  
}

uint32_t RCC_GetClockSourceFreq(RCC_ClockSource src) {
    (void)src;
    return 16000000UL; // fixed HSI16-like value for tests
}

void NVIC_EnableIRQ(IRQn_Type IRQn) {
    (void)IRQn;  
}

void NVIC_DisableIRQ(IRQn_Type IRQn) {
    (void)IRQn;  
}

void NVIC_SetPriority(IRQn_Type IRQn, IRQn_Priority priority) {
    (void)IRQn;
    (void)priority;  
}

void NVIC_ClearPendingIRQ(IRQn_Type IRQn) {
    (void)IRQn;  
}
