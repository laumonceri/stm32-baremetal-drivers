#include <stdarg.h>
#include <setjmp.h>
#include <cmocka.h>
#include "mmio_stub.h"

#include "../drivers/uart/include/uart_driver.h"

static void test_uart_init(void **state) {
    (void)state;
    mmio_reset();
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