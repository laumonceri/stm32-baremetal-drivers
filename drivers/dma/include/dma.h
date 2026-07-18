#ifndef DMA_H
#define DMA_H

#include <stddef.h>
#include "stm32_dma_hw.h"
#include "rcc.h"

typedef enum {
    DMA_DISABLE = 0,
    DMA_ENABLE
} dma_enable;

typedef enum {
    DMA_DIR_READ_FROM_PERIPHERAL = 0, // peripheral -> memory
    DMA_DIR_READ_FROM_MEMORY          // memory -> peripheral
} dma_data_transfer_direction;

typedef enum {
    DMA_SIZE_8B = 0,
    DMA_SIZE_16B,
    DMA_SIZE_32B
} dma_data_size;

typedef enum {
    DMA_PRIORITY_LVL_LOW = 0,
    DMA_PRIORITY_LVL_MEDIUM,
    DMA_PRIORITY_LVL_HIGH,
    DMA_PRIORITY_LVL_VERY_HIGH
} dma_priority_level;

// TODO: make this more user-friendly -> the user does not need to see the register table
// to set the dma -> easier to setup?
typedef struct {
    RCC_AHB1ENR_Pos peripheral;

    uint32_t base; /* DMA1_BASE or DMA2_BASE*/
    uint32_t channel; /* Channel from 1 to 7 */
    uint32_t request; /* CSELR value, 0-7, see RM0394's per-channel request mapping table */

    uint32_t peripheral_address; /* CPAR: peripheral data register, or one of the two buffers in mem2mem */
    uint32_t memory_address;     /* CMAR: memory buffer, or the other buffer in mem2mem */
    uint32_t data_count;         /* CNDTR: number of data items to transfer */

    dma_enable en;
    dma_enable tcie;
    dma_enable htie;
    dma_enable teie;
    dma_data_transfer_direction dir;
    dma_enable circ;
    dma_enable pinc;
    dma_enable minc;
    dma_data_size psize;
    dma_data_size msize;
    dma_priority_level pl;
    dma_enable mem2mem;
} dma_config_t;

typedef enum {
    DMA_OK = 0,
    DMA_ERROR_NULL_DEVICE,
    DMA_ERROR_INVALID_BASE,
    DMA_ERROR_INVALID_CHANNEL
} DMA_Status;

DMA_Status DMA_Config(dma_config_t *cfg);

#endif /* DMA_H */
