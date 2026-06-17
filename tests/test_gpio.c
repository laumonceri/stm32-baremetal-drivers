#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "mmio_stub.h"

#include "../drivers/gpio/include/gpio.h"

static void test_gpio_init_returns_state_returnserror(void **state) {
    (void) state;
    mmio_reset();

    assert_int_equal(gpio_pin_cfg_init(NULL, GPIO_PORT_B, PIN_3,
                                       GPIO_MODE_INPUT, GPIO_PUSH_PULL,
                                       GPIO_SPEED_LOW, GPIO_NOPULL, AF_0),
                     GPIO_ERROR_NULL_CONFIG);
    
    // Invalid PORT
    gpio_pin_cfg_t cfg;                 
    assert_int_equal(gpio_pin_cfg_init(&cfg, 8, PIN_3,
                                       GPIO_MODE_INPUT, GPIO_PUSH_PULL,
                                       GPIO_SPEED_LOW, GPIO_NOPULL, AF_0),
                     GPIO_ERROR_INVALID_PORT);
    assert_int_equal(gpio_pin_cfg_init(&cfg, -8, PIN_3,
                                       GPIO_MODE_INPUT, GPIO_PUSH_PULL,
                                       GPIO_SPEED_LOW, GPIO_NOPULL, AF_0),
                     GPIO_ERROR_INVALID_PORT);

    // Invalid PIN
    assert_int_equal(gpio_pin_cfg_init(&cfg, GPIO_PORT_B, 16,
                                       GPIO_MODE_INPUT, GPIO_PUSH_PULL,
                                       GPIO_SPEED_LOW, GPIO_NOPULL, AF_0),
                     GPIO_ERROR_INVALID_PIN);
    assert_int_equal(gpio_pin_cfg_init(&cfg, GPIO_PORT_B, -1,
                                       GPIO_MODE_INPUT, GPIO_PUSH_PULL,
                                       GPIO_SPEED_LOW, GPIO_NOPULL, AF_0),
                     GPIO_ERROR_INVALID_PIN);
    
    // Invalid AF
    assert_int_equal(gpio_pin_cfg_init(&cfg, GPIO_PORT_B, PIN_3,
                                       GPIO_MODE_AF, GPIO_PUSH_PULL,
                                       GPIO_SPEED_LOW, GPIO_NOPULL, 16),
                     GPIO_ERROR_INVALID_AF);
    assert_int_equal(gpio_pin_cfg_init(&cfg, GPIO_PORT_B, PIN_3,
                                       GPIO_MODE_AF, GPIO_PUSH_PULL,
                                       GPIO_SPEED_LOW, GPIO_NOPULL, -1),
                     GPIO_ERROR_INVALID_AF);
    
    // Valid config
    assert_int_equal(gpio_pin_cfg_init(&cfg, GPIO_PORT_B, PIN_3,
                                       GPIO_MODE_INPUT, GPIO_PUSH_PULL,
                                       GPIO_SPEED_LOW, GPIO_NOPULL, AF_0),
                     GPIO_OK);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_gpio_init_returns_state_returnserror),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}