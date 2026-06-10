#include "button.h"
#include "exti.h"

// configure the pin and any internal pull-up if needed.
void Button_Init(const Pin_Config *button) {
    // Buttons are inputs for the board
    GPIO_Init(button->port, button->pin, GPIO_MODE_INPUT, GPIO_NOPULL);
}

bool Button_EventPending(void) {
    return button_event;
}

void Button_ClearEvent(void) {
    button_event = false;
}

// returns debounced state.
// If pressed returns 1 if not 0
Button_State Button_GetState(const Pin_Config *button) {
    // Debounce
    for (volatile int i = 0; i < 5000; i++) __asm__("nop");

    return (GPIO_ReadPin(button->port, button->pin) == GPIO_PIN_SET) ? BUTTON_PRESSED : BUTTON_RELEASED;
}

void Button_EnableInterrupt(const Pin_Config *button)
{
    EXTI_ConfigLine(button, EXTI_TRIGGER_FALLING);
    EXTI_EnableIRQ(EXTI15_10_IRQ, 2); // IRQ number + priority
}

