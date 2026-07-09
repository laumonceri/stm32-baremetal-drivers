#ifndef STM32_RCC_HW
#define STM32_RCC_HW

#include "stm32_mmio.h"

#define RCC_BASE             0x40021000UL

#define RCC_CR_OFFSET        0x00
#define RCC_CFGR_OFFSET      0x08
#define RCC_AHB2ENR_OFFSET   0x4C
#define RCC_APB1RSTR1_OFFSET 0x38
#define RCC_APB1ENR1_OFFSET  0x58
#define RCC_APB2ENR_OFFSET   0x60
#define RCC_CCIPR_OFFSET     0x88
#define RCC_APB1ENR2_OFFSET  0x5C

#define RCC_CR               REG32(RCC_BASE + RCC_CR_OFFSET)
#define RCC_CFGR             REG32(RCC_BASE + RCC_CFGR_OFFSET) // Clock configuration register
#define RCC_AHB2ENR          REG32(RCC_BASE + RCC_AHB2ENR_OFFSET)
#define RCC_APB1RSTR1        REG32(RCC_BASE + RCC_APB1RSTR1_OFFSET)
#define RCC_APB1ENR1         REG32(RCC_BASE + RCC_APB1ENR1_OFFSET)
#define RCC_APB2ENR          REG32(RCC_BASE + RCC_APB2ENR_OFFSET)
#define RCC_CCIPR            REG32(RCC_BASE + RCC_CCIPR_OFFSET)
#define RCC_APB1ENR2         REG32(RCC_BASE + RCC_APB1ENR2_OFFSET)

#define RCC_CR_MSION        BIT(0) // MSI clock enable
#define RCC_CR_MSIRDY       BIT(1) // MSI clock ready flag
#define RCC_CR_MSIPLLON     BIT(2) // MSI clock PLL enable
#define RCC_CR_MSIRGSEL     BIT(3) // MSI clock range selection

/* MSI clock range bits [7:4] */
#define RCC_CR_MSIRANGE_Pos            4U
#define RCC_CR_MSIRANGE_Msk            (0xFU << RCC_CR_MSIRANGE_Pos)
#define RCC_CR_MSIRANGE(n)             (((n) << RCC_CR_MSIRANGE_Pos) & RCC_CR_MSIRANGE_Msk)

#define RCC_CR_HSION        BIT(8) // HSI clock enable
#define RCC_CR_HSIKERON     BIT(9) // HSI16 always enable for peripheral kernels
#define RCC_CR_HSIRDY       BIT(10) // HSI clock ready flag
#define RCC_CR_HSIASFS      BIT(11) // HSI16 automatic start from Stop

#define RCC_CR_HSEON        BIT(16) // HSE clock enable
#define RCC_CR_HSERDY       BIT(17) // HSE clock ready flag
#define RCC_CR_HSEBYP       BIT(18) // HSE crystal oscillator bypass
#define RCC_CR_CSSON        BIT(19) // HSE clock security system enable

#define RCC_CR_PLLON        BIT(24) // Main PLL enable
#define RCC_CR_PLLRDY       BIT(25) // Main PLL clock ready flag
#define RCC_CR_PLLSAI1ON    BIT(26) // SAI1 PLL enable
#define RCC_CR_PLLSAI1RDY   BIT(27) // SAI1 PLL clock ready flag

#endif
