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
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_gpio_init_returns_state_returnserror),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}