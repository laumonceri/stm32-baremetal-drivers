#include "spi.h"
#include "systick.h"

#define USER_LED 13 // PB13 -> LED4 USER - GREEN

static const spi_dev_t SPI = {.clk =
                                  {
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
                                  .crc_en = SPI_CRC_ENABLE,
                                  .crc_poly = 0x07, // reset value
                                  .ssm = SPI_SW_SLAVE_ENABLE,
                                  .mstr = SPI_MASTER_CONFIG,

                                  .ds = SPI_DATA_8B,

                                  .mosi = {.port = GPIO_PORT_A,
                                           .pin = PIN_7,
                                           .mode = GPIO_MODE_AF,
                                           .speed = GPIO_SPEED_HIGH,
                                           .pull = GPIO_NOPULL,
                                           .af = AF_5},
                                  .miso = {.port = GPIO_PORT_A,
                                           .pin = PIN_6,
                                           .mode = GPIO_MODE_AF,
                                           .pull = GPIO_NOPULL,
                                           .af = AF_5},
                                  .sck = {.port = GPIO_PORT_A,
                                          .pin = PIN_5,
                                          .mode = GPIO_MODE_AF,
                                          .speed = GPIO_SPEED_HIGH,
                                          .pull = GPIO_NOPULL,
                                          .af = AF_5},
                                  .nss =
                                      {
                                          .port = GPIO_PORT_A,
                                          .pin = PIN_4,
                                          .mode = GPIO_MODE_OUTPUT,
                                          .speed = GPIO_SPEED_HIGH,
                                          .pull = GPIO_NOPULL,
                                          .af = AF_0 // no-op
                                      },
                              }};

int main(void) {
  RCC_SetSysclk(RCC_SYSCLK_HSI16);
  SysTick_Init(16000);

  SPI_Config(&SPI);
  
  gpio_pin_cfg_t led;
  GPIO_pin_cfg_init(&led, GPIO_PORT_B, USER_LED, GPIO_MODE_OUTPUT,
                    GPIO_PUSH_PULL, GPIO_SPEED_LOW, GPIO_NOPULL, AF_0);
  GPIO_ConfigPin(&led);

  static const uint8_t test_bytes[] = {0xCA, 0xFE, 0xB0, 0x55};
  const size_t test_bytes_len = sizeof(test_bytes);

  while (1) {
    int data_ok = 1;

    SPI_Select(&SPI);
    for (size_t i = 0U; i < test_bytes_len - 1U; i++) {
      uint8_t received = SPI_TransferByte(&SPI, test_bytes[i]);
      if (received != test_bytes[i]) {
        data_ok = 0;
      }
    }
    uint8_t last_received = SPI_TransferLastByteWithCRC(
        &SPI, test_bytes[test_bytes_len - 1U]);
    if (last_received != test_bytes[test_bytes_len - 1U]) {
      data_ok = 0;
    }
    SPI_Status crc_status = SPI_CheckAndClearCRCError(&SPI);
    SPI_Deselect(&SPI);

    if (data_ok && crc_status == SPI_OK) {
      /* Loopback + CRC both confirmed: blink to show it's working. */
      GPIO_WritePin(&led, GPIO_PIN_SET);
      SysTick_Delay(1000);
      GPIO_WritePin(&led, GPIO_PIN_RESET);
      SysTick_Delay(1000);
    } else {
      /* Data mismatch or CRC error: change LED pattern to blink faster so
       * a failure is visibly distinct from a working blink. To test the CRC,
       * unplug the wire jumper mid-run. */
      GPIO_WritePin(&led, GPIO_PIN_SET);
      SysTick_Delay(100); // faster blink
      GPIO_WritePin(&led, GPIO_PIN_RESET);
      SysTick_Delay(100);

    }
  }
}