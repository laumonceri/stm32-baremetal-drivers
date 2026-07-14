#include "st7735.h"
#include "systick.h"

static void ST7735_gpio_init(const st7735_dev_t *dev) {
  GPIO_ConfigPin(&dev->res);
  GPIO_ConfigPin(&dev->dc);
}

void ST7735_SendCommand(const st7735_dev_t *dev, uint8_t cmd) {
  SPI_Select(dev->spi);
  GPIO_WritePin(&dev->dc, GPIO_PIN_RESET);
  SPI_TransferByte(dev->spi, cmd);
  SPI_Deselect(dev->spi);
}

void ST7735_SendCommandWithParams(const st7735_dev_t *dev, uint8_t cmd,
                                  const uint8_t *params, uint32_t len) {
  SPI_Select(dev->spi);

  GPIO_WritePin(&dev->dc, GPIO_PIN_RESET); /* DC low = command byte */
  SPI_TransferByte(dev->spi, cmd);

  GPIO_WritePin(&dev->dc, GPIO_PIN_SET); /* DC high = parameter/data bytes */
  for (uint32_t i = 0; i < len; i++) {
    SPI_TransferByte(dev->spi, params[i]);
  }

  SPI_Deselect(dev->spi);
}

/* TODO: the 10ms/120ms values here are placeholders, not yet checked
 * against the datasheet's own Reset timing section - verify before
 * relying on them. */
static void ST7735_Reset(const st7735_dev_t *dev) {
  GPIO_WritePin(&dev->res, GPIO_PIN_RESET);
  SysTick_Delay(10);
  GPIO_WritePin(&dev->res, GPIO_PIN_SET);
  SysTick_Delay(120);
}

/* Brings the display up */
void ST7735_Init(const st7735_dev_t *dev) {
  ST7735_gpio_init(dev);
  ST7735_Reset(dev);

  ST7735_SendCommand(dev, ST7735_CMD_SLPOUT);
  SysTick_Delay(120); /* SLPOUT restriction */

  /* Frame rate control: normal/idle/partial mode, datasheet default
   * timings. Required on most clones - reset defaults aren't guaranteed
   * to produce a visible image on their own. */
  uint8_t frmctr1_params[3] = {0x01U, 0x2CU, 0x2DU};
  ST7735_SendCommandWithParams(dev, ST7735_CMD_FRMCTR1, frmctr1_params, 3);

  uint8_t frmctr2_params[3] = {0x01U, 0x2CU, 0x2DU};
  ST7735_SendCommandWithParams(dev, ST7735_CMD_FRMCTR2, frmctr2_params, 3);

  uint8_t frmctr3_params[6] = {0x01U, 0x2CU, 0x2DU, 0x01U, 0x2CU, 0x2DU};
  ST7735_SendCommandWithParams(dev, ST7735_CMD_FRMCTR3, frmctr3_params, 6);

  uint8_t invctr_param = 0x07U;
  ST7735_SendCommandWithParams(dev, ST7735_CMD_INVCTR, &invctr_param, 1);

  /* Power control: GVDD/GVCL and boost levels. */
  uint8_t pwctr1_params[3] = {0xA2U, 0x02U, 0x84U};
  ST7735_SendCommandWithParams(dev, ST7735_CMD_PWCTR1, pwctr1_params, 3);

  uint8_t pwctr2_param = 0xC5U;
  ST7735_SendCommandWithParams(dev, ST7735_CMD_PWCTR2, &pwctr2_param, 1);

  uint8_t pwctr3_params[2] = {0x0AU, 0x00U};
  ST7735_SendCommandWithParams(dev, ST7735_CMD_PWCTR3, pwctr3_params, 2);

  uint8_t pwctr4_params[2] = {0x8AU, 0x2AU};
  ST7735_SendCommandWithParams(dev, ST7735_CMD_PWCTR4, pwctr4_params, 2);

  uint8_t pwctr5_params[2] = {0x8AU, 0xEEU};
  ST7735_SendCommandWithParams(dev, ST7735_CMD_PWCTR5, pwctr5_params, 2);

  /* VCOM level. */
  uint8_t vmctr1_param = 0x0EU;
  ST7735_SendCommandWithParams(dev, ST7735_CMD_VMCTR1, &vmctr1_param, 1);

  /* Make sure the panel isn't left in an inverted-color state from reset. */
  ST7735_SendCommand(dev, ST7735_CMD_INVOFF);

  /* Set the Interface Pixel Format to 16-bit/pixel (RGB565) */
  uint8_t colmod_param = 0x55U;
  ST7735_SendCommandWithParams(dev, ST7735_CMD_COLMOD, &colmod_param, 1);

  /* set 0x00U for rgb | 0x08U for grb */
  uint8_t rgb_param = 0x08U;
  ST7735_SendCommandWithParams(dev, ST7735_CMD_MADCTL, &rgb_param, 1);

  /* Gamma correction: positive and negative polarity. */
  uint8_t gmctrp1_params[16] = {0x02U, 0x1CU, 0x07U, 0x12U, 0x37U, 0x32U,
                                 0x29U, 0x2DU, 0x29U, 0x25U, 0x2BU, 0x39U,
                                 0x00U, 0x01U, 0x03U, 0x10U};
  ST7735_SendCommandWithParams(dev, ST7735_CMD_GMCTRP1, gmctrp1_params, 16);

  uint8_t gmctrn1_params[16] = {0x03U, 0x1DU, 0x07U, 0x06U, 0x2EU, 0x2CU,
                                 0x29U, 0x2DU, 0x2EU, 0x2EU, 0x37U, 0x3FU,
                                 0x00U, 0x00U, 0x02U, 0x10U};
  ST7735_SendCommandWithParams(dev, ST7735_CMD_GMCTRN1, gmctrn1_params, 16);

  ST7735_SendCommand(dev, ST7735_CMD_NORON);
  SysTick_Delay(10);

  /* Turn on the display */
  ST7735_SendCommand(dev, ST7735_CMD_DISPON);
  SysTick_Delay(120);
}

void ST7735_DrawImage(const st7735_dev_t *dev, const uint16_t *pixels) {
  /* Column window: XS=0x0000, XE=0x007F (0-127, full 128px width) */
  uint8_t caset_params[4] = {0x00U, 0x00U, 0x00U, 0x7FU};
  ST7735_SendCommandWithParams(dev, ST7735_CMD_CASET, caset_params, 4);

  /* Row window: YS=0x0000, YE=0x007F (0-127, full 128px height) */
  uint8_t raset_params[4] = {0x00U, 0x00U, 0x00U, 0x7FU};
  ST7735_SendCommandWithParams(dev, ST7735_CMD_RASET, raset_params, 4);

  /* Same continuous CS-low RAMWR write as FillScreen, but reading a real
   * pixel array instead of repeating one color. */
  SPI_Select(dev->spi);

  GPIO_WritePin(&dev->dc, GPIO_PIN_RESET);
  SPI_TransferByte(dev->spi, ST7735_CMD_RAMWR);

  GPIO_WritePin(&dev->dc, GPIO_PIN_SET); /* DC high = pixel data */
  for (uint32_t i = 0; i < 128UL * 128UL; i++) {
    SPI_TransferByte(dev->spi, (uint8_t)(pixels[i] >> 8));
    SPI_TransferByte(dev->spi, (uint8_t)(pixels[i] & 0xFFU));
  }

  SPI_Deselect(dev->spi);
}

void ST7735_FillScreen(const st7735_dev_t *dev, uint16_t color) {
  /* Column window: XS=0x0000, XE=0x007F (0-127, full 128px width) */
  uint8_t caset_params[4] = {0x00U, 0x00U, 0x00U, 0x7FU};
  ST7735_SendCommandWithParams(dev, ST7735_CMD_CASET, caset_params, 4);

  /* Row window: YS=0x0000, YE=0x007F (0-127, full 128px height) */
  uint8_t raset_params[4] = {0x00U, 0x00U, 0x00U, 0x7FU};
  ST7735_SendCommandWithParams(dev, ST7735_CMD_RASET, raset_params, 4);

  /* RAMWR + 128*128 repeated pixels, all as one continuous CS-low write -
   * too much data (32KB) to build as a params[] array, so this streams
   * it directly instead of going through ST7735_SendCommandWithParams. */
  SPI_Select(dev->spi);

  GPIO_WritePin(&dev->dc, GPIO_PIN_RESET);
  SPI_TransferByte(dev->spi, ST7735_CMD_RAMWR);

  GPIO_WritePin(&dev->dc, GPIO_PIN_SET); /* DC high = pixel data */
  uint8_t hi = (uint8_t)(color >> 8);
  uint8_t lo = (uint8_t)(color & 0xFFU);
  for (uint32_t i = 0; i < (128UL * 128UL); i++) {
    SPI_TransferByte(dev->spi, hi);
    SPI_TransferByte(dev->spi, lo);
  }

  SPI_Deselect(dev->spi);
}