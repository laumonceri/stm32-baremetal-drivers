#ifndef ST7735_H
#define ST7735_H

#include "spi.h"

typedef struct {
    const spi_dev_t *spi;
    gpio_pin_cfg_t res;
    gpio_pin_cfg_t dc;
} st7735_dev_t;

/* Command opcodes from the ST7735 datasheet command table */
#define ST7735_CMD_SLPOUT  0x11U // Sleep out & booster on
#define ST7735_CMD_INVOFF  0x20U // Display inversion off
#define ST7735_CMD_NORON   0x13U // Normal display mode on
#define ST7735_CMD_COLMOD  0x3AU // Interface pixel format
#define ST7735_CMD_CASET   0x2AU // Column address set
#define ST7735_CMD_RASET   0x2BU // Row address set
#define ST7735_CMD_RAMWR   0x2CU // Memory write
#define ST7735_CMD_DISPON  0x29U // Display on
#define ST7735_CMD_MADCTL  0x36U // Memory Data Access Control
#define ST7735_CMD_FRMCTR1 0xB1U // Frame rate control: normal mode
#define ST7735_CMD_FRMCTR2 0xB2U // Frame rate control: idle mode
#define ST7735_CMD_FRMCTR3 0xB3U // Frame rate control: partial mode
#define ST7735_CMD_INVCTR  0xB4U // Display inversion control
#define ST7735_CMD_PWCTR1  0xC0U // Power control 1
#define ST7735_CMD_PWCTR2  0xC1U // Power control 2
#define ST7735_CMD_PWCTR3  0xC2U // Power control 3: normal mode
#define ST7735_CMD_PWCTR4  0xC3U // Power control 4: idle mode
#define ST7735_CMD_PWCTR5  0xC4U // Power control 5: partial mode
#define ST7735_CMD_VMCTR1  0xC5U // VCOM control 1
#define ST7735_CMD_GMCTRP1 0xE0U // Gamma '+' polarity correction
#define ST7735_CMD_GMCTRN1 0xE1U // Gamma '-' polarity correction

/**
 * @brief Send a command byte with no parameters (DC low, one byte, CS
 * released). Matches commands like SLPOUT/DISPON that take no data.
 */
void ST7735_SendCommand(const st7735_dev_t *dev, uint8_t cmd);

/**
 * @brief Send a command byte followed by its parameter bytes, all as one
 * continuous CS-low transaction (DC low for the opcode, DC high for every
 * parameter byte that follows). Needed for CASET/RASET/RAMWR, where
 * toggling CS between bytes would break the transfer.
 */
void ST7735_SendCommandWithParams(const st7735_dev_t *dev, uint8_t cmd,
                                   const uint8_t *params, uint32_t len);

/**
 * @brief Configure RES/DC pins and bring the panel all the way up: SLPOUT,
 * frame-rate/power/VCOM/gamma registers, COLMOD, MADCTL, then DISPON.
 */
void ST7735_Init(const st7735_dev_t *dev);

/**
 * @brief Fill the entire 128x128 screen with one RGB565 color.
 */
void ST7735_FillScreen(const st7735_dev_t *dev, uint16_t color);

/**
 * @brief Draw a full 128x128 RGB565 image. `pixels` must point to exactly
 * 128*128 uint16_t values, in row-major order.
 */
void ST7735_DrawImage(const st7735_dev_t *dev, const uint16_t *pixels);

#endif /* ST7735_H */
