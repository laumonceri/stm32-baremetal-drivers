#include <stdarg.h>
#include <setjmp.h>
#include <cmocka.h>
#include "mmio_stub.h"

#include "../drivers/uart/include/uart.h"

static void test_uart_init(void **state) {
    (void)state;
    mmio_reset();

    uart_handle_t h;

    uart_device_t dev = {
        .clk = {
            .bus = RCC_UART_BUS_APB1,
            .enr = {
                .apb1 = RCC_APB1ENR1_USART2,
            },
            .clk_sel = RCC_SEL_USART2,
            .clk_src = RCC_CLK_HSI16,
        },
        .uart = {
            .base = USART2_BASE,
            .irq = USART2_IRQn,
            .word_length = UART_WORD_LENGTH_8B,
            .parity = UART_PARITY_NONE,
            .stop_bits = UART_STOP_1,
            .baudrate = 115200,

            .tx = {.port = GPIO_PORT_A,
                    .pin = PIN_2,
                    .mode = GPIO_MODE_AF,
                    .pull = GPIO_PULL_UP,
                    .af = AF_7},
             .rx = {.port = GPIO_PORT_A,
                    .pin = PIN_3,
                    .mode = GPIO_MODE_AF,
                    .pull = GPIO_PULL_DOWN,
                    .af = AF_7}
        }
    };

    assert_int_equal(UART_Init(NULL, &dev), UART_ERROR_NULL_HANDLE);

    assert_int_equal(UART_Init(&h, NULL), UART_ERROR_NULL_DEVICE);

    dev.uart.base = GPIOA_BASE;
    assert_int_equal(UART_Init(&h, &dev), UART_ERROR_INVALID_BASE);

    // Invalid port
    dev.uart.base = USART2_BASE; // reset to valid base
    dev.uart.tx.port = -0xDEAD;
    assert_int_equal(UART_Init(&h, &dev), UART_ERROR_INVALID_CONFIG);

    dev.uart.rx.port = -0xDEAD;
    assert_int_equal(UART_Init(&h, &dev), UART_ERROR_INVALID_CONFIG);

    dev.uart.tx.port = 0xDEAD;
    assert_int_equal(UART_Init(&h, &dev), UART_ERROR_INVALID_CONFIG);

    dev.uart.rx.port = 0xDEAD;
    assert_int_equal(UART_Init(&h, &dev), UART_ERROR_INVALID_CONFIG);

    // Invalid pin
    dev.uart.tx.port = GPIO_PORT_A;
    dev.uart.rx.port = GPIO_PORT_A;

    dev.uart.tx.pin = -0xDEAD;
    assert_int_equal(UART_Init(&h, &dev), UART_ERROR_INVALID_CONFIG);

    dev.uart.rx.pin = -0xDEAD;
    assert_int_equal(UART_Init(&h, &dev), UART_ERROR_INVALID_CONFIG);

    dev.uart.tx.pin = 0xDEAD;
    assert_int_equal(UART_Init(&h, &dev), UART_ERROR_INVALID_CONFIG);

    dev.uart.rx.pin = 0xDEAD;
    assert_int_equal(UART_Init(&h, &dev), UART_ERROR_INVALID_CONFIG);

    dev.uart.tx.pin = PIN_2;
    dev.uart.rx.pin = PIN_3;

    // Invalid IRQ
    dev.uart.irq = EXTI0_IRQn;
    assert_int_equal(UART_Init(&h, &dev), UART_ERROR_INVALID_IRQ);

    dev.uart.irq = -0xDEAD;
    assert_int_equal(UART_Init(&h, &dev), UART_ERROR_INVALID_IRQ);

    dev.uart.irq = USART2_IRQn;
    assert_int_equal(UART_Init(&h, &dev), UART_OK);

    // UART_InstanceInUse: a second handle can't claim an IRQ already
    // registered to a different handle.
    uart_handle_t h2;
    assert_int_equal(UART_Init(&h2, &dev), UART_ERROR_INSTANCE_IN_USE);

    // UART_clock_init
    assert_int_equal(UART_Init(&h, &dev), UART_OK);
    assert_int_equal(RCC_APB1ENR, 1U << dev.clk.enr.apb1);

    dev.clk.bus = RCC_UART_BUS_APB2;
    assert_int_equal(UART_Init(&h, &dev), UART_OK);
    assert_int_equal(RCC_APB2ENR, 1U << dev.clk.enr.apb2);

    // UART_gpio_init
    // assert_int_equal(UART_Init(&h, &dev), UART_OK);


}

static void test_uart_deinit(void **state) {
    (void)state;
    mmio_reset();
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_uart_init),
        cmocka_unit_test(test_uart_deinit)
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}