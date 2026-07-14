#include "st7735.h"
#include "systick.h"

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
                                  .crc_en = SPI_CRC_DISABLE,
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

static const st7735_dev_t dev = {.spi = &SPI,
                                 .res =
                                     {
                                         .port = GPIO_PORT_B,
                                         .pin = PIN_0,
                                         .mode = GPIO_MODE_OUTPUT,
                                         .speed = GPIO_SPEED_LOW,
                                         .pull = GPIO_NOPULL,
                                         .af = AF_0 // no-op, plain GPIO output
                                     },
                                 .dc = {
                                     .port = GPIO_PORT_B,
                                     .pin = PIN_1,
                                     .mode = GPIO_MODE_OUTPUT,
                                     .speed = GPIO_SPEED_LOW,
                                     .pull = GPIO_NOPULL,
                                     .af = AF_0 // no-op, plain GPIO output
                                 }};

int main(void) {

  RCC_SetSysclk(RCC_SYSCLK_HSI16);
  SysTick_Init(
      16000); /* HSI16 = 16MHz core clock, no AHB prescale -> 16000 ticks/ms */

  SPI_Config(&SPI);

  ST7735_Init(&dev);

  ST7735_FillScreen(&dev, 0xF800); // RGB565 red
  while (1) {
    //
  }
}