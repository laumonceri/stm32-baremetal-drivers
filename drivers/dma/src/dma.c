#include "dma.h"

static DMA_Status DMA_validate_base(const dma_config_t *dma) {
    DMA_Status ret;
    switch (dma->base) {
        case (DMA1_BASE):
        /* fallthrough */
        case (DMA2_BASE):
            ret = DMA_OK;
            break;
        default:
            ret = DMA_ERROR_INVALID_BASE;
            break;
    }
    return ret;
}

static DMA_Status DMA_validate_channel(const dma_config_t *dma) {
    if (dma->channel < 1 || dma->channel > 7) {
        return DMA_ERROR_INVALID_CHANNEL;
    }
    return DMA_OK;
}

static void DMA_SelectChannel(dma_config_t *dma) {
    DMA_CSELR(dma->base) = (DMA_CSELR(dma->base) & ~DMA_CSELR_Msk(dma->channel))
                | DMA_CSELR_SEL(dma->channel, dma->request);
}

static void DMA_SetPeripheralAddress(dma_config_t *dma) {
    DMA_CPAR(dma->base, dma->channel) = dma->peripheral_address;
}

static void DMA_SetMemoryAddress(dma_config_t *dma) {
    DMA_CMAR(dma->base, dma->channel) = dma->memory_address;
}

static void DMA_SetNumberDataTransfer(dma_config_t *dma) {
    DMA_CNDTR(dma->base, dma->channel) = dma->data_count;
}

static void DMA_SetControlRegister(dma_config_t *dma) {
    uint32_t ccr = 0;

    if (dma->tcie == DMA_ENABLE)               ccr |= DMA_CCR_TCIE;
    if (dma->htie == DMA_ENABLE)               ccr |= DMA_CCR_HTIE;
    if (dma->teie == DMA_ENABLE)               ccr |= DMA_CCR_TEIE;
    if (dma->dir == DMA_DIR_READ_FROM_MEMORY)  ccr |= DMA_CCR_DIR;
    if (dma->circ == DMA_ENABLE)                ccr |= DMA_CCR_CIRC;
    if (dma->pinc == DMA_ENABLE)                ccr |= DMA_CCR_PINC;
    if (dma->minc == DMA_ENABLE)                ccr |= DMA_CCR_MINC;
    if (dma->mem2mem == DMA_ENABLE)              ccr |= DMA_CCR_MEM2MEM;

    ccr |= DMA_CCR_PSIZE(dma->psize);
    ccr |= DMA_CCR_MSIZE(dma->msize);
    ccr |= DMA_CCR_PL(dma->pl);

    DMA_CCR(dma->base, dma->channel) = ccr;
}

DMA_Status DMA_Config(dma_config_t *dma) {
    if (dma == NULL) {
        return DMA_ERROR_NULL_DEVICE;
    }

    DMA_Status check_base = DMA_validate_base(dma);
    if (check_base != DMA_OK) {
        return check_base;
    }

    DMA_Status check_channel = DMA_validate_channel(dma);
    if (check_channel != DMA_OK) {
        return check_channel;
    }

    RCC_EnableAHB1(dma->peripheral);

    /* CCR fields other than EN can't be changed while the channel is
       running, disable first in case it was left enabled by a prior
       config. */
    DMA_CCR(dma->base, dma->channel) &= ~DMA_CCR_EN;

    DMA_SetControlRegister(dma);
    DMA_SetPeripheralAddress(dma);
    DMA_SetMemoryAddress(dma);
    DMA_SetNumberDataTransfer(dma);
    DMA_SelectChannel(dma);

    if (dma->en == DMA_ENABLE) {
        DMA_CCR(dma->base, dma->channel) |= DMA_CCR_EN;
    }

    return DMA_OK;
}
