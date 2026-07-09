#include "exti.h"

volatile bool button_event = false;

// This is a hardware interrupt config, because we depend on the button event
void EXTI_ConfigLine(const gpio_pin_cfg_t *cfg, EXTI_Trigger trigger) {
  uint32_t mask = 1U << EXTI_REG_BIT(cfg->pin);

  EXTI_RTSR(EXTI_REG_INDEX(cfg->pin)) &= ~mask;
  EXTI_FTSR(EXTI_REG_INDEX(cfg->pin)) &= ~mask;

  // 1. Configure the corresponding mask bit in the EXTI_IMR register.
  EXTI_IMR(EXTI_REG_INDEX(cfg->pin)) |= mask;

  // 2. Configure the Trigger Selection bits of the Interrupt
  // line (EXTI_RTSR) -> Rising Edge
  switch (trigger) {
  case EXTI_TRIGGER_RISING:
    EXTI_RTSR(EXTI_REG_INDEX(cfg->pin)) |= mask;
    break;
  case EXTI_TRIGGER_FALLING:
    EXTI_FTSR(EXTI_REG_INDEX(cfg->pin)) |= mask;
    break;
  case EXTI_TRIGGER_BOTH:
    EXTI_RTSR(EXTI_REG_INDEX(cfg->pin)) |= mask;
    EXTI_FTSR(EXTI_REG_INDEX(cfg->pin)) |= mask;
    break;
  default:
    /* Invalid trigger type */
    break;
  }
}

// Clear pending ==  Selected trigger request occurred
void EXTI_ClearPending(uint8_t line) {
  EXTI_PR(EXTI_REG_INDEX(line)) = (1UL << EXTI_REG_BIT(line));
}

// If we do not clear PR1, the hardware thinks the interrupt is still active.
// The ISR will fire continuously (re-enter immediately).
void EXTI15_10_IRQHandler(void) {
  if (EXTI_PR(EXTI_REG_INDEX(13)) & (1UL << EXTI_REG_BIT(13))) {
    EXTI_ClearPending(13);
    button_event = true;
  }
}
