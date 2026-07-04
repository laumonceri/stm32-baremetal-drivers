#ifndef SYSCFG_H
#define SYSCFG_H

#include "stm32_syscfg_hw.h"
#include "rcc.h"
#include "gpio.h"
    
void SYSCFG_EnableClock(void);
void SYSCFG_ConfigEXTI(const gpio_pin_cfg_t *cfg);

#endif /* SYSCFG_H */