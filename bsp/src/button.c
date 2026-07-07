#include "button.h"

// configure the pin and any internal pull-up if needed.
void Button_Init(const gpio_pin_cfg_t *button_cfg) {
    // Buttons are inputs for the board, so we configure the pin as input with pull-up (active low button)
    (void)GPIO_ConfigPin(button_cfg);
}

bool Button_EventPending(void) {
    return button_event;
}

void Button_ClearEvent(void) {
    button_event = false;
}

// returns debounced state.
// If pressed returns 1 if not 0
Button_State Button_GetState(const gpio_pin_cfg_t *button_cfg) {
    // Debounce
    for (volatile int i = 0; i < 5000; i++) __asm__("nop");

    GPIO_PinState out_state;
    GPIO_ReadPin(button_cfg, &out_state);
    return (out_state == GPIO_PIN_SET) ? BUTTON_PRESSED : BUTTON_RELEASED;
}

void Button_EnableInterrupt(const gpio_pin_cfg_t *button_cfg)
{
    // Configure SYSCFG (for button GPIO)
    SYSCFG_EnableClock();
    SYSCFG_ConfigEXTI(button_cfg);

    EXTI_ClearPending(button_cfg->pin);
    EXTI_ConfigLine(button_cfg, EXTI_TRIGGER_FALLING);

    NVIC_ClearPendingIRQ(EXTI15_10_IRQn);
    NVIC_SetPriority(EXTI15_10_IRQn, 1); // IRQ number + priority
    NVIC_EnableIRQ(EXTI15_10_IRQn); // IRQ number

}

