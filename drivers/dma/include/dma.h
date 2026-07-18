#ifndef DMA_H
#define DMA_H

#include "stm32_dma_hw.h"

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

typedef struct {
    // clock
    // pin
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
} DMA_Status;


#endif /* DMA_H */