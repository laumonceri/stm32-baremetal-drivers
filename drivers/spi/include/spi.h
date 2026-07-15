#ifndef SPI_H
#define SPI_H

#include <stdint.h>
#include "stm32_spi_hw.h"
#include "stm32_rcc_hw.h"
#include "gpio.h"
#include "nvic.h"
#include "rcc.h"

typedef enum {
    SPI_CLK_2 = 0, // fastest
    SPI_CLK_4,
    SPI_CLK_8,
    SPI_CLK_16,
    SPI_CLK_32,
    SPI_CLK_64,
    SPI_CLK_128,
    SPI_CLK_256 //slowest
} spi_baud_rate;

typedef enum {
    SPI_CPHA_FIRST = 0,
    SPI_CPHA_SECOND
} spi_clock_phase;

typedef enum {
    SPI_CPOL_IDLE_LOW = 0,
    SPI_CPOL_IDLE_HIGH
} spi_clock_polarity;

typedef enum {
    SPI_DATA_MSB_FIRST = 0,
    SPI_DATA_LSB_FIRST
} spi_frame_format;

typedef enum {
    SPI_CRC_8B = 0,
    SPI_CRC_16B
} spi_crc_length;

typedef enum {
    SPI_CRC_DISABLE = 0,
    SPI_CRC_ENABLE
} spi_crc_enable;

typedef enum {
    SPI_SW_SLAVE_DISABLE = 0,
    SPI_SW_SLAVE_ENABLE
} spi_sw_slave_management;

typedef enum {
    SPI_SLAVE_CONFIG = 0,
    SPI_MASTER_CONFIG
} spi_master_selection;

typedef enum {
    SPI_FIFO_EMPTY = 0,
    SPI_FIFO_QUARTER_FULL,
    SPI_FIFO_HALF_FULL,
    SPI_FIFO_FULL
} spi_fifo_lvl;

typedef enum {
    SPI_DATA_4B = 3,
    SPI_DATA_5B,
    SPI_DATA_6B,
    SPI_DATA_7B,
    SPI_DATA_8B,
    SPI_DATA_9B,
    SPI_DATA_10B,
    SPI_DATA_11B,
    SPI_DATA_12B,
    SPI_DATA_13B,
    SPI_DATA_14B,
    SPI_DATA_15B,
    SPI_DATA_16B
} spi_data_size;

typedef struct {
    uint32_t base;
    IRQn_Type irq;
    spi_baud_rate baudrate;
    spi_clock_phase cpha;
    spi_clock_polarity cpol;
    spi_frame_format frame_format;
    spi_crc_length crc_len;
    spi_crc_enable crc_en;
    uint16_t crc_poly;
    spi_sw_slave_management ssm;
    spi_master_selection mstr;

    spi_data_size ds;

    gpio_pin_cfg_t mosi;
    gpio_pin_cfg_t miso;
    gpio_pin_cfg_t sck;
    gpio_pin_cfg_t nss;
} spi_config_t;

typedef struct {
    RCC_Bus bus;
    rcc_enr_t enr;
} rcc_spi_clk_t;

typedef struct {
    rcc_spi_clk_t clk;
    spi_config_t spi;
} spi_dev_t;

typedef enum {
    SPI_OK = 0,
    SPI_ERROR_NULL_DEVICE,
    SPI_ERROR_INVALID_BASE,
    SPI_ERROR_INVALID_CRC
} SPI_Status;

SPI_Status SPI_Config(const spi_dev_t *dev);

/**
 * @brief Pull NSS/CS low, claiming the bus for this slave.
 */
void SPI_Select(const spi_dev_t *dev);

/**
 * @brief Release NSS/CS high, ending the transaction.
 */
void SPI_Deselect(const spi_dev_t *dev);

/**
 * @brief Transfer one byte, full-duplex.
 *
 * SPI is inherently full-duplex: writing a byte to send always clocks a
 * byte in too, even if the caller only cares about one direction.
 *
 * @param dev  Configured SPI device
 * @param data Byte to transmit
 * @return Byte received during the same transfer
 */
uint8_t SPI_TransferByte(const spi_dev_t *dev, uint8_t data);

/**
 * @brief Transfer the last byte of a CRC-checked message.
 *
 * Sets CRCNEXT before writing the byte so the hardware appends a CRC
 * frame right after it, then reads and discards that CRC frame (call
 * SPI_CheckAndClearCRCError() afterward to see whether it matched).
 * Only call this for the final byte of a message; use SPI_TransferByte()
 * for every byte before it.
 *
 * @param dev  Configured SPI device, with crc_en = SPI_CRC_ENABLE
 * @param data Last byte to transmit
 * @return Byte received during the same transfer
 */
uint8_t SPI_TransferLastByteWithCRC(const spi_dev_t *dev, uint8_t data);

/**
 * @brief Check whether the last CRC-checked message had a CRC mismatch,
 * clearing the error flag unconditionally so it never leaks into the
 * next transfer's check.
 *
 * @param dev Configured SPI device
 * @return SPI_ERROR_INVALID_CRC if CRCERR was set, SPI_OK otherwise
 */
SPI_Status SPI_CheckAndClearCRCError(const spi_dev_t *dev);

#endif /* SPI_H */