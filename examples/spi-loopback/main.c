#include "spi.h"

#define USER_LED 13 // PB13 -> LED4 USER - GREEN
#define LOOPBACK_TEST_BYTE 0xA5

static void dummy_wait(uint32_t iterations) {
    for (volatile uint32_t i = 0; i < iterations; i++) {
        __asm__("nop");
    }
}

static const spi_dev_t SPI = {
    .clk = {
        .bus = RCC_BUS_APB2,
        .enr = {.apb2 = RCC_APB2ENR_SPI1},
    },

    .spi = {
        .base = SPI1_BASE,
        .irq = SPI1_IRQn,
        .baudrate = SPI_CLK_32,
        .cpha = SPI_CPHA_FIRST,
        .cpol = SPI_CPOL_IDLE_LOW,
        .frame_format = SPI_DATA_MSB_FIRST,
        .crc_len = SPI_CRC_8B,
        .crc_en = SPI_CRC_DISABLE,
        .ssm = SPI_SW_SLAVE_ENABLE,
        .mstr = SPI_MASTER_CONFIG,

        .ds = SPI_DATA_8B,

        .mosi = {
            .port = GPIO_PORT_A,
            .pin = PIN_7,
            .mode = GPIO_MODE_AF,
            .speed = GPIO_SPEED_HIGH,
            .pull =  GPIO_NOPULL,
            .af = AF_5
        },
        .miso = {
            .port = GPIO_PORT_A,
            .pin = PIN_6,
            .mode = GPIO_MODE_AF,
            .pull =  GPIO_NOPULL,
            .af = AF_5
        },
        .sck = {
            .port = GPIO_PORT_A,
            .pin = PIN_5,
            .mode = GPIO_MODE_AF,
            .speed = GPIO_SPEED_HIGH,
            .pull =  GPIO_NOPULL,
            .af = AF_5
        },
        .nss = {
            .port = GPIO_PORT_A,
            .pin = PIN_4,
            .mode = GPIO_MODE_OUTPUT,
            .speed = GPIO_SPEED_HIGH,
            .pull =  GPIO_NOPULL,
            .af = AF_0 // no-op
        },
    }
};

int main(void) {
    RCC_SetSysclk(RCC_SYSCLK_HSI16);

    SPI_Config(&SPI);

    /* Jumper PA7 (MOSI) to PA6 (MISO) on the board: this proves the
     * driver's TX/RX path is correct before wiring up any real slave. */
    gpio_pin_cfg_t led;
    GPIO_pin_cfg_init(&led, GPIO_PORT_B, USER_LED, GPIO_MODE_OUTPUT,
                      GPIO_PUSH_PULL, GPIO_SPEED_LOW, GPIO_NOPULL, AF_0);
    GPIO_ConfigPin(&led);

    while (1) {
        SPI_Select(&SPI);
        uint8_t received = SPI_TransferByte(&SPI, LOOPBACK_TEST_BYTE);
        SPI_Deselect(&SPI);

        if (received == LOOPBACK_TEST_BYTE) {
            /* Loopback confirmed: blink to show the driver is working. */
            GPIO_WritePin(&led, GPIO_PIN_SET);
            dummy_wait(100000);
            GPIO_WritePin(&led, GPIO_PIN_RESET);
            dummy_wait(100000);
        } else {
            /* Mismatch: leave the LED off so a failure is visibly distinct
             * from a working blink, instead of failing silently. */
            GPIO_WritePin(&led, GPIO_PIN_RESET);
        }
    }
}