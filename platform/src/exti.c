#include "exti.h"

volatile bool button_event = false; 

// These bits are written by software to select the source input for the EXTI13
// external interrupt. 
static void SYSCFG_Config(const Pin_Config *pin) {
    RCC_APB2ENR |= (1 << 0); // SYSCFGEN
    // Map port to EXTI line dynamically
    uint8_t shift = (pin->pin % 4) * 4;
    SYSCFG_EXTICR4 &= ~(0xFU << shift);
    SYSCFG_EXTICR4 |= ((pin->port & 0xF) << shift);
}

// Nested Vectored Interrupt Controller - CPU's interrupt manager
static void NVIC_Setup(uint8_t irq, uint8_t priority) {
    // Enable EXTI13 interrupt in NVIC
    NVIC_ISER1 |= (1 << (irq - 32));

    // Set priority -> Lower number = higher priority
    NVIC_IPR1 = (NVIC_IPR1 & ~(0xFF << 16)) | (priority << 16);
}

// This is a hardware interrupt config, because we depend on the button event
void EXTI_ConfigLine(const Pin_Config *pin, EXTI_Trigger trigger) {
    // Configure SYSCFG (for button GPIO)
   // RCC_AHB2_EnableClock(button_pin->port);
    SYSCFG_Config(pin);

    // 1. Configure the corresponding mask bit in the EXTI_IMR register.
    EXTI_IMR1 |= (1 << pin->pin);

    // 2. Configure the Trigger Selection bits of the Interrupt 
    // line (EXTI_RTSR) -> Rising Edge
    if (trigger == EXTI_TRIGGER_RISING || trigger == EXTI_TRIGGER_BOTH)
        EXTI_RTSR1 |= (1 << pin->pin);
    if (trigger == EXTI_TRIGGER_FALLING || trigger == EXTI_TRIGGER_BOTH)
        EXTI_FTSR1 |= (1 << pin->pin);

}

void EXTI_EnableIRQ(uint8_t irq, uint8_t priority) {
    NVIC_Setup(irq, priority);
}

// Clear pending ==  Selected trigger request occurred
void EXTI_ClearPending(uint8_t line) {
    EXTI_PR1 |= (1 << line);
}

// If we do not clear PR1, the hardware thinks the interrupt is still active.
// The ISR will fire continuously (re-enter immediately).
void EXTI15_10_IRQHandler(void) {
    if (EXTI_PR1 & (1U << 13)) {
        EXTI_ClearPending(13);
        button_event = true;      // Set flag for main loop
    }
}
