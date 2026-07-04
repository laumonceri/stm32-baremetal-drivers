#include "button.h"
#include "led.h"

int main(void)
{
    gpio_pin_cfg_t led_cfg;
    GPIO_Status led_st =
    gpio_pin_cfg_init(&led_cfg, GPIO_PORT_B, PIN_13, GPIO_MODE_OUTPUT,
                        GPIO_PUSH_PULL, GPIO_SPEED_LOW, GPIO_NOPULL, AF_0);
    if (led_st != GPIO_OK){
        return -1; // Handle error
    }
    LED_Init(&led_cfg);
    
    gpio_pin_cfg_t button_cfg;
    GPIO_Status button_st =
    gpio_pin_cfg_init(&button_cfg, GPIO_PORT_C, PIN_13, GPIO_MODE_INPUT,
                  GPIO_PUSH_PULL, GPIO_SPEED_LOW, GPIO_PULL_DOWN, AF_0);
    if (button_st != GPIO_OK){
        return -1; // Handle error
    }
    Button_Init(&button_cfg);
    // Enable button interrupt
    Button_EnableInterrupt(&button_cfg);

   

    while (1) {
        if (Button_EventPending()) {
            // Handle button press event
            Button_ClearEvent();
            // Do something in response to the button press
            LED_On(&led_cfg);
        }
        // Button_ClearEvent();
        //LED_Off(&led_cfg);
    }
}