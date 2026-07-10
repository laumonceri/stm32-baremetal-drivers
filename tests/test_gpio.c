#include <stdarg.h>
#include <setjmp.h>
#include <cmocka.h>

#include "mmio_stub.h"

#include "../drivers/gpio/include/gpio.h"

static void test_gpio_pin_cfg_init(void **state) {
    (void)state;
    mmio_reset();

    assert_int_equal(GPIO_pin_cfg_init(NULL, GPIO_PORT_A, PIN_3,
                                       GPIO_MODE_INPUT, GPIO_PUSH_PULL,
                                       GPIO_SPEED_LOW, GPIO_NOPULL, AF_0),
                     GPIO_ERROR_NULL_CONFIG);

    // Invalid PORT
    gpio_pin_cfg_t cfg;
    assert_int_equal(GPIO_pin_cfg_init(&cfg, 0xdead, PIN_3,
                                       GPIO_MODE_INPUT, GPIO_PUSH_PULL,
                                       GPIO_SPEED_LOW, GPIO_NOPULL, AF_0),
                     GPIO_ERROR_INVALID_PORT);
    assert_int_equal(GPIO_pin_cfg_init(&cfg, -8, PIN_3,
                                       GPIO_MODE_INPUT, GPIO_PUSH_PULL,
                                       GPIO_SPEED_LOW, GPIO_NOPULL, AF_0),
                     GPIO_ERROR_INVALID_PORT);

    // Invalid PIN
    assert_int_equal(GPIO_pin_cfg_init(&cfg, GPIO_PORT_B, 16,
                                       GPIO_MODE_INPUT, GPIO_PUSH_PULL,
                                       GPIO_SPEED_LOW, GPIO_NOPULL, AF_0),
                     GPIO_ERROR_INVALID_PIN);
    assert_int_equal(GPIO_pin_cfg_init(&cfg, GPIO_PORT_C, -1,
                                       GPIO_MODE_INPUT, GPIO_PUSH_PULL,
                                       GPIO_SPEED_LOW, GPIO_NOPULL, AF_0),
                     GPIO_ERROR_INVALID_PIN);

    // Invalid AF
    assert_int_equal(GPIO_pin_cfg_init(&cfg, GPIO_PORT_D, PIN_3,
                                       GPIO_MODE_AF, GPIO_PUSH_PULL,
                                       GPIO_SPEED_LOW, GPIO_NOPULL, 16),
                     GPIO_ERROR_INVALID_AF);
    assert_int_equal(GPIO_pin_cfg_init(&cfg, GPIO_PORT_E, PIN_3,
                                       GPIO_MODE_AF, GPIO_PUSH_PULL,
                                       GPIO_SPEED_LOW, GPIO_NOPULL, -1),
                     GPIO_ERROR_INVALID_AF);

    // Valid config + base address for every port
    static const struct { GPIO_Port port; uint32_t expected_base; } cases[] = {
        { GPIO_PORT_A, GPIOA_BASE },
        { GPIO_PORT_B, GPIOB_BASE },
        { GPIO_PORT_C, GPIOC_BASE },
        { GPIO_PORT_D, GPIOD_BASE },
        { GPIO_PORT_E, GPIOE_BASE },
        { GPIO_PORT_F, GPIOF_BASE },
        { GPIO_PORT_G, GPIOG_BASE },
        { GPIO_PORT_H, GPIOH_BASE },
    };

    for (size_t i = 0; i < sizeof(cases) / sizeof(cases[0]); ++i) {
        assert_int_equal(GPIO_pin_cfg_init(&cfg, cases[i].port, PIN_3,
                                           GPIO_MODE_INPUT, GPIO_PUSH_PULL,
                                           GPIO_SPEED_LOW, GPIO_NOPULL, AF_0),
                         GPIO_OK);
        assert_int_equal(cfg.base, cases[i].expected_base);
    }
}

static void test_gpio_configpin(void **state) {
    (void)state;
    mmio_reset();

    // NULL config
    assert_int_equal(GPIO_ConfigPin(NULL), GPIO_ERROR_NULL_CONFIG);

    // Invalid PORT
    gpio_pin_cfg_t cfg = {
        .port = 0xdead,
        .pin = PIN_3,
        .mode = GPIO_MODE_INPUT,
        .otype = GPIO_PUSH_PULL,
        .speed = GPIO_SPEED_LOW,
        .pull = GPIO_NOPULL,
        .af = AF_0,
        .base = GPIOB_BASE
    };
    assert_int_equal(GPIO_ConfigPin(&cfg), GPIO_ERROR_INVALID_PORT);

    // Invalid PIN
    cfg.port = GPIO_PORT_H;
    cfg.pin = 0xdead;
    assert_int_equal(GPIO_ConfigPin(&cfg), GPIO_ERROR_INVALID_PIN);

    // Invalid AF
    cfg.pin = PIN_3;
    cfg.mode = GPIO_MODE_AF;
    cfg.af = 0xdead;
    assert_int_equal(GPIO_ConfigPin(&cfg), GPIO_ERROR_INVALID_AF);

    // Valid config
    cfg.af = AF_0;
    assert_int_equal(GPIO_ConfigPin(&cfg), GPIO_OK);

    assert_int_equal((GPIO_MODER(cfg.base) >> (cfg.pin * 2U)) & 0x3U, cfg.mode);
    assert_int_equal((GPIO_OTYPER(cfg.base) >> cfg.pin) & 0x1U, cfg.otype);
    assert_int_equal((GPIO_OSPEEDR(cfg.base) >> (cfg.pin * 2U)) & 0x3U, cfg.speed);
    assert_int_equal((GPIO_PUPDR(cfg.base) >> (cfg.pin * 2U)) & 0x3U, cfg.pull);
    assert_int_equal((GPIO_AFRL(cfg.base) >> (cfg.pin * 4U)) & 0xFU, cfg.af);

    // Valid config on a pin >= 8 -> exercises the AFRH branch of GPIO_setAF
    cfg.pin = PIN_10;
    cfg.af = AF_7;
    assert_int_equal(GPIO_ConfigPin(&cfg), GPIO_OK);

    uint32_t afrh_idx = cfg.pin - 8U;
    assert_int_equal((GPIO_AFRH(cfg.base) >> (afrh_idx * 4U)) & 0xFU, cfg.af);
}

static void test_gpio_writepin(void **state) {
    (void)state;
    mmio_reset();

    assert_int_equal(GPIO_WritePin(NULL, GPIO_PIN_RESET), GPIO_ERROR_NULL_CONFIG);

    gpio_pin_cfg_t cfg = {
        .port = GPIO_PORT_B,
        .pin = PIN_3,
        .mode = GPIO_MODE_INPUT,
        .otype = GPIO_PUSH_PULL,
        .speed = GPIO_SPEED_LOW,
        .pull = GPIO_NOPULL,
        .af = AF_0,
        .base = GPIOB_BASE
    };

    // Invalid state
    assert_int_equal(GPIO_WritePin(&cfg, 0xdead), GPIO_ERROR_INVALID_STATE);

    assert_int_equal(GPIO_WritePin(&cfg, GPIO_PIN_SET), GPIO_OK);
    assert_int_equal(GPIO_BSRR(cfg.base), (1U << cfg.pin));

    assert_int_equal(GPIO_WritePin(&cfg, GPIO_PIN_RESET), GPIO_OK);
    assert_int_equal(GPIO_BSRR(cfg.base), (1U << (cfg.pin + 16U)));

}

static void test_gpio_readpin(void **state) {
    (void)state;
    mmio_reset();

    GPIO_PinState out_state;
    assert_int_equal(GPIO_ReadPin(NULL, GPIO_PIN_RESET), GPIO_ERROR_NULL_CONFIG);

    gpio_pin_cfg_t cfg = {
        .port = GPIO_PORT_B,
        .pin = PIN_10,
        .mode = GPIO_MODE_INPUT,
        .otype = GPIO_PUSH_PULL,
        .speed = GPIO_SPEED_LOW,
        .pull = GPIO_NOPULL,
        .af = AF_0,
        .base = GPIOB_BASE
    };

    assert_int_equal(GPIO_ReadPin(&cfg, NULL), GPIO_ERROR_NULL_CONFIG);

    // Pin bit clear -> RESET
    GPIO_IDR(cfg.base) = 0;
    assert_int_equal(GPIO_ReadPin(&cfg, &out_state), GPIO_OK);
    assert_int_equal(out_state, GPIO_PIN_RESET);

    // Pin bit set -> SET
    GPIO_IDR(cfg.base) = (1U << cfg.pin);
    assert_int_equal(GPIO_ReadPin(&cfg, &out_state), GPIO_OK);
    assert_int_equal(out_state, GPIO_PIN_SET);

    // Neighboring bits set, cfg.pin bit still clear -> must stay RESET
    GPIO_IDR(cfg.base) = (1U << (cfg.pin + 1)) | (1U << (cfg.pin - 1));
    assert_int_equal(GPIO_ReadPin(&cfg, &out_state), GPIO_OK);
    assert_int_equal(out_state, GPIO_PIN_RESET);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_gpio_pin_cfg_init),
        cmocka_unit_test(test_gpio_configpin),
        cmocka_unit_test(test_gpio_writepin),
        cmocka_unit_test(test_gpio_readpin)
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}