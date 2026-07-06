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
