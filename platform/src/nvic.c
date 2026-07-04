#include "nvic.h"

void NVIC_EnableIRQ(IRQn_Type IRQn) {
    if (IRQn < 0) {
        return;
    }

    NVIC_ISER(NVIC_REG_INDEX(IRQn)) |= (1U << NVIC_REG_BIT(IRQn));
}

void NVIC_DisableIRQ(IRQn_Type IRQn)
{
    if (IRQn < 0) {
        return;
    }
    NVIC_ICER(NVIC_REG_INDEX(IRQn)) |= (1U << NVIC_REG_BIT(IRQn));
}

void NVIC_SetPendingIRQ(IRQn_Type IRQn)
{
    if (IRQn < 0) {
        return;
    }

    NVIC_ISPR(NVIC_REG_INDEX(IRQn)) |= (1U << NVIC_REG_BIT(IRQn));
}

void NVIC_ClearPendingIRQ(IRQn_Type IRQn)
{
    if (IRQn < 0) {
        return;
    }

    NVIC_ICPR(NVIC_REG_INDEX(IRQn)) |= (1U << NVIC_REG_BIT(IRQn));
}
uint32_t NVIC_GetPendingIRQ(IRQn_Type IRQn) {
    if (IRQn < 0) {
        return 1;
    }

    return (NVIC_ISPR(NVIC_REG_INDEX(IRQn)) & (1U << NVIC_REG_BIT(IRQn))) != 0;
}

void NVIC_SetPriority(IRQn_Type IRQn, IRQn_Priority priority)
{
    if (IRQn < 0) {
        return;
    }

    uint32_t reg = NVIC_IPR_BASE + ((uint32_t)IRQn / 4U) * 4U;
    uint32_t shift = ((uint32_t)IRQn % 4U) * 8U;

    REG32(reg) &= ~(0xFFU << shift);
    REG32(reg) |= ((priority & 0x0FU) << (shift + 4U));
}

uint32_t NVIC_GetPriority(IRQn_Type IRQn) {
    return NVIC_IPR(NVIC_REG_INDEX(IRQn)) & (1U << NVIC_REG_BIT(IRQn));
}