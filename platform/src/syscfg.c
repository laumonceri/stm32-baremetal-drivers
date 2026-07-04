#include "syscfg.h"

void SYSCFG_EnableClock(void)
{
    RCC_EnableAPB2(RCC_APB2ENR_SYSCFG);
}
void SYSCFG_ConfigEXTI(const gpio_pin_cfg_t *cfg)
{
    SYSCFG_EXTICR(cfg->pin / 4) &= ~(0xFU << ((cfg->pin % 4) * 4));
    SYSCFG_EXTICR(cfg->pin / 4) |= ((cfg->port & 0xFU) << ((cfg->pin % 4) * 4));
}