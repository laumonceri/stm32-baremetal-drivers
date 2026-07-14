#ifndef ST7735_H
#define ST7735_H

#include "spi.h"

typedef struct {
    const spi_dev_t *spi;
    gpio_pin_cfg_t res;
    gpio_pin_cfg_t dc;
} st7735_dev_t;

/* Command opcodes from the ST7735 datasheet command table */
#define ST7735_CMD_SLPOUT 0x11U // Sleep out & booster on
#define ST7735_CMD_COLMOD 0x3AU // Interface pixel format
#define ST7735_CMD_CASET  0x2AU // Column address set
#define ST7735_CMD_RASET  0x2BU // Row address set
#define ST7735_CMD_RAMWR  0x2CU // Memory write 
#define ST7735_CMD_DISPON 0x29U // Display on
#define ST7735_CMD_MADCTL 0x36U

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
 * @brief Configure RES/DC pins and bring the panel up through SLPOUT.
 * Incomplete: still needs COLMOD + DISPON once those opcodes are verified.
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
