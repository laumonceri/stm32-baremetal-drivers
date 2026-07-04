#ifndef STM32_SYSTICK_HW_H
#define STM32_SYSTICK_HW_H

#include "stm32_mmio.h"

// From Cortex-M4 User Guide
#define SYST_CSR   REG32(0xE000E010)  // RW Control and Status Register
#define SYST_RVR   REG32(0xE000E014)  // RW Reload Value Register
#define SYST_CVR   REG32(0xE000E018)  // RW Current Value Register
#define SYST_CALIB REG32(0xE000E01C)  // RO Calibration Value Register 

#define SYST_CSR_ENABLE    (1U << 0) 
#define SYST_CSR_TICKINT   (1U << 1)
#define SYST_CSR_CLKSOURCE (1U << 2)


#endif