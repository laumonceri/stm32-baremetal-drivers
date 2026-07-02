#include "nvic.h"

//static inline volatile uint32_t *
//nvic_bit_reg(uint32_t base, IRQn_Type IRQn, uint32_t *bit_pos)
//{
//    uint32_t irq = (uint32_t)IRQn;

//    uint32_t reg_index = irq / 32U;
//    *bit_pos = irq % 32U;

 //   return (volatile uint32_t *)(base + (reg_index * 4U));
//}

//static inline volatile uint32_t *
//nvic_prio_reg(uint32_t irq, uint32_t *byte_shift)
//{
//    uint32_t reg_index = irq / 4U;
//    *byte_shift = (irq % 4U) * 8U;

//    return (volatile uint32_t *)(NVIC_IPR_BASE + (reg_index * 4U));
//}

void NVIC_EnableIRQ(IRQn_Type IRQn) {
    if (IRQn < 0) {
        return;
    }

    NVIC_ISER(NVIC_REG_INDEX(IRQn)) |= (1U << NVIC_REG_BIT(IRQn));

    //uint32_t bit;
    //volatile uint32_t *iser = 
    //    nvic_bit_reg(NVIC_ISER_BASE, IRQn, &bit);

    // *iser = (1U << bit);
}

void NVIC_DisableIRQ(IRQn_Type IRQn)
{
    if (IRQn < 0) {
        return;
    }
    NVIC_ICER(NVIC_REG_INDEX(IRQn)) |= (1U << NVIC_REG_BIT(IRQn));
    

    //uint32_t bit;
    //volatile uint32_t *icer =
    //    nvic_bit_reg(NVIC_ICER_BASE, IRQn, &bit);

    // *icer = (1U << bit);
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

    NVIC_IPR(NVIC_REG_INDEX(IRQn)) |= (1U << NVIC_REG_BIT(IRQn));

    //uint32_t irq = (uint32_t)IRQn;
    //uint32_t shift;

    //volatile uint32_t *ipr =
    //    nvic_prio_reg(irq, &shift);

    /* Clear priority byte */
    //*ipr &= ~(0xFFU << shift);

    /* STM32L4: only upper 4 bits implemented */
    //*ipr |= ((priority & 0x0FU) << (shift + 4U));
}

uint32_t NVIC_GetPriority(IRQn_Type IRQn) {
    return NVIC_IPR(NVIC_REG_INDEX(IRQn)) & (1U << NVIC_REG_BIT(IRQn));
}