#include "spi.h"

static void SPI_clock_init(const spi_dev_t *dev) {
  RCC_EnablePeripheralClock(dev->clk.bus, dev->clk.enr);
}

static void SPI_gpio_init(const spi_dev_t *dev) {
  GPIO_ConfigPin(&dev->spi.mosi);
  GPIO_ConfigPin(&dev->spi.miso);
  GPIO_ConfigPin(&dev->spi.sck);
  GPIO_ConfigPin(&dev->spi.nss);
}

/* Write to SPI_CR1 registers */
static void SPI_SetBaudRate(const spi_dev_t *dev) {
  SPI_CR1(dev->spi.base) &= ~SPI_CR1_BR_Msk;
  SPI_CR1(dev->spi.base) |= SPI_CR1_BR(dev->spi.baudrate);
}

static void SPI_SetClockPhase(const spi_dev_t *dev) {
  SPI_CR1(dev->spi.base) &= ~SPI_CR1_CPHA;

  if (dev->spi.cpha == SPI_CPHA_SECOND) {
    SPI_CR1(dev->spi.base) |= SPI_CR1_CPHA;
  }
}

static void SPI_SetClockPolarity(const spi_dev_t *dev) {
  SPI_CR1(dev->spi.base) &= ~SPI_CR1_CPOL;

  if (dev->spi.cpol == SPI_CPOL_IDLE_HIGH) {
    SPI_CR1(dev->spi.base) |= SPI_CR1_CPOL;
  }
}

static void SPI_SetFrameFormat(const spi_dev_t *dev) {
  SPI_CR1(dev->spi.base) &= ~SPI_CR1_LSBFIRST;

  if (dev->spi.frame_format == SPI_DATA_LSB_FIRST) {
    SPI_CR1(dev->spi.base) |= SPI_CR1_LSBFIRST;
  }
}

static void SPI_SetCRCLength(const spi_dev_t *dev) {
  SPI_CR1(dev->spi.base) &= ~SPI_CR1_CRCL;

  if (dev->spi.crc_len == SPI_CRC_16B) {
    SPI_CR1(dev->spi.base) |= SPI_CR1_CRCL;
  }
}

static void SPI_EnableDisableCRC(const spi_dev_t *dev) {
  SPI_CR1(dev->spi.base) &= ~SPI_CR1_CRCEN;

  if (dev->spi.crc_en == SPI_CRC_ENABLE) {
    SPI_CR1(dev->spi.base) |= SPI_CR1_CRCEN;
  }
}

static void SPI_ManageSwSlave(const spi_dev_t *dev) {
  SPI_CR1(dev->spi.base) &= ~SPI_CR1_SSM;
  SPI_CR1(dev->spi.base) &= ~SPI_CR1_SSI;

  if (dev->spi.ssm == SPI_SW_SLAVE_ENABLE) {
    /* Software slave management enabled */
    SPI_CR1(dev->spi.base) |= SPI_CR1_SSM;
    /*  Internal slave select */
    SPI_CR1(dev->spi.base) |= SPI_CR1_SSI;
  }
}

static void SPI_SelectMaster(const spi_dev_t *dev) {
  SPI_CR1(dev->spi.base) &= ~SPI_CR1_MSTR;

  if (dev->spi.mstr == SPI_MASTER_CONFIG) {
    SPI_CR1(dev->spi.base) |= SPI_CR1_MSTR;
  }
}

/* Write to SPI_CR2 registers */
static void SPI_SetDataSize(const spi_dev_t *dev) {
  SPI_CR2(dev->spi.base) &= ~SPI_CR2_DS_Msk;
  SPI_CR2(dev->spi.base) |= SPI_CR2_DS(dev->spi.ds);
}

static void SPI_SetFIFOThreshold(const spi_dev_t *dev) {
  SPI_CR2(dev->spi.base) &= ~SPI_CR2_FRXTH;

  if (dev->spi.ds <= SPI_DATA_8B) {
    SPI_CR2(dev->spi.base) |= SPI_CR2_FRXTH;
  }
}

static void SPI_SetCRC(const spi_dev_t *dev) {
  if (dev->spi.crc_en == SPI_CRC_ENABLE) {
    SPI_CRCPR(dev->spi.base) = dev->spi.crc_poly;
  }
}

// TODO: Read 42.4.8 Procedure for enabling SPI
static void SPI_Enable(const spi_dev_t *dev) {
  SPI_CR1(dev->spi.base) |= SPI_CR1_SPE;
}

static void SPI_Disable(const spi_dev_t *dev) {
  /* 1. Wait until FTLVL[1:0] = 00 (no more data to transmit) */
  while ((SPI_SR(dev->spi.base) & SPI_SR_FTLVL_Msk) !=
         SPI_SR_FTLVL(SPI_FIFO_EMPTY))
    ;
  /* 2. Wait until BSY=0 (the last data frame is processed) */
  while ((SPI_SR(dev->spi.base) & SPI_SR_BSY) != 0)
    ;

  /* 3. Disable the SPI (SPE=0) */
  SPI_CR1(dev->spi.base) &= ~SPI_CR1_SPE;

  /* 4. Read data until FRLVL[1:0] = 00 (read all the received data) */
  while ((SPI_SR(dev->spi.base) & SPI_SR_FRLVL_Msk) !=
         SPI_SR_FRLVL(SPI_FIFO_EMPTY)) {
    (void)SPI_DR(dev->spi.base);
  }
}

SPI_Status SPI_Config(const spi_dev_t *dev) {
  if (dev == NULL) {
    return SPI_ERROR_NULL_DEVICE;
  }

  SPI_clock_init(dev);

  SPI_Disable(dev);

  SPI_gpio_init(dev);
  SPI_SetBaudRate(dev);
  SPI_SetClockPolarity(dev);
  SPI_SetClockPhase(dev);
  SPI_SetFrameFormat(dev);
  SPI_ManageSwSlave(dev);
  SPI_SelectMaster(dev);
  SPI_SetDataSize(dev);
  SPI_SetFIFOThreshold(dev);

  SPI_Enable(dev);

  return SPI_OK;
}

void SPI_Select(const spi_dev_t *dev) {
  GPIO_WritePin(&dev->spi.nss, GPIO_PIN_RESET);
}

void SPI_Deselect(const spi_dev_t *dev) {
  GPIO_WritePin(&dev->spi.nss, GPIO_PIN_SET);
}

uint8_t SPI_TransferByte(const spi_dev_t *dev, uint8_t data) {
  while ((SPI_SR(dev->spi.base) & SPI_SR_TXE) == 0)
    ;
  SPI_DR8(dev->spi.base) = data;

  while ((SPI_SR(dev->spi.base) & SPI_SR_RXNE) == 0)
    ;
  return SPI_DR8(dev->spi.base);
}
