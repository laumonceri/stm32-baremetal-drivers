#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "mmio_stub.h"

#include "../drivers/gpio/include/gpio.h"

static void test_gpio_write_sets_bsrr(void **state) {
    (void) state;
    mmio_reset();

    gpio_pin_cfg_t cfg;
    assert_int_equal(gpio_pin_cfg_init(&cfg, GPIO_PORT_A, PIN_0,
                                       GPIO_MODE_OUTPUT, GPIO_PUSH_PULL,
                                       GPIO_SPEED_LOW, GPIO_NOPULL, AF_0),
                     GPIO_OK);

    /* configure pin (enables clock and writes registers) */
    assert_int_equal(GPIO_ConfigPin(&cfg), GPIO_OK);

    /* set pin */
    assert_int_equal(GPIO_WritePin(&cfg, GPIO_PIN_SET), GPIO_OK);
    assert_int_equal(REG32(cfg.base + GPIO_BSRR_OFFSET), (1U << PIN_0));

    /* reset pin */
    assert_int_equal(GPIO_WritePin(&cfg, GPIO_PIN_RESET), GPIO_OK);
    assert_int_equal(REG32(cfg.base + GPIO_BSRR_OFFSET), (1U << (PIN_0 + 16U)));
}

static void test_gpio_read_returns_state(void **state) {
    (void) state;
    mmio_reset();

    gpio_pin_cfg_t cfg;
    assert_int_equal(gpio_pin_cfg_init(&cfg, GPIO_PORT_B, PIN_3,
                                       GPIO_MODE_INPUT, GPIO_PUSH_PULL,
                                       GPIO_SPEED_LOW, GPIO_NOPULL, AF_0),
                     GPIO_OK);

    /* simulate input high */
    REG32(cfg.base + GPIO_IDR_OFFSET) = (1U << PIN_3);

    GPIO_PinState st;
    assert_int_equal(GPIO_ReadPin(&cfg, &st), GPIO_OK);
    assert_int_equal(st, GPIO_PIN_SET);

    /* simulate input low */
    REG32(cfg.base + GPIO_IDR_OFFSET) = 0;
    assert_int_equal(GPIO_ReadPin(&cfg, &st), GPIO_OK);
    assert_int_equal(st, GPIO_PIN_RESET);
}

static void test_gpio_init_returns_state_returnserror(void **state) {
    (void) state;
    mmio_reset();

    assert_int_equal(gpio_pin_cfg_init(NULL, GPIO_PORT_B, PIN_3,
                                       GPIO_MODE_INPUT, GPIO_PUSH_PULL,
                                       GPIO_SPEED_LOW, GPIO_NOPULL, AF_0),
                     GPIO_ERROR_NULL_CONFIG);

    gpio_pin_cfg_t cfg;
    GPIO_Port 
    assert_int_equal(gpio_pin_cfg_init(&cfg, GPIO_PORT_B, PIN_3,
                                       GPIO_MODE_INPUT, GPIO_PUSH_PULL,
                                       GPIO_SPEED_LOW, GPIO_NOPULL, AF_0),
                     GPIO_OK);

}

// Validation / error paths:
// gpio_pin_cfg_init: NULL cfg, invalid port/pin, invalid AF.
// GPIO_ReadPin: NULL out param returns error.
// Behavior / branches:
// GPIO_ConfigPin sets MODER/OTYPER/OSPEEDR/PUPDR bits for output and input.
// AF selection: pin 7 (AFRL) and pin 8 (AFRH) cases.
// GPIO_WritePin: BSRR lower half for SET, upper half for RESET.
// GPIO_ReadPin: IDR high/low.
// Edge cases:
// PIN_0 and PIN_15 boundary bit math.
// Repeated writes not clobbering unused bits.
// Interaction:
// RCC_EnableAHB2Clock called for the correct port (stub records calls).

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_gpio_write_sets_bsrr),
        cmocka_unit_test(test_gpio_read_returns_state),
        cmocka_unit_test(test_gpio_init_returns_state_returnserror),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}