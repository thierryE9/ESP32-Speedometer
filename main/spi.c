#include "spi.h"

void init_spi(display_t *dev) {
    spi_bus_config_t spi_cfg = {
        .mosi_io_num = GPIO_MOSI,
        .miso_io_num = GPIO_MISO,
        .sclk_io_num = GPIO_SCLK,
        .quadwp_io_num = GPIO_QUADWP,
        .quadhd_io_num = GPIO_QUADHD
    };

    spi_bus_initialize(SPI1_HOST, &spi_cfg, SPI_DMA_CH_AUTO);

    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = SPI_FREQUENCY,
        .queue_size = 7,
        .mode = 0,
        .flags = SPI_DEVICE_NO_DUMMY,
        .spics_io_num = GPIO_CS_LCD
    };
    spi_device_handle_t handle;
    spi_bus_add_device(SPI1_HOST, &devcfg, &handle);

    dev->_SPIHandle = handle;

}

void spi_write_byte(spi_device_handle_t SPIHandle, const uint8_t data, size_t dataLength) {
    spi_transaction_t SPITransaction;

    if(dataLength > 0) {
        memset(&SPITransaction, 0, sizeof(spi_transaction_t));
        SPITransaction.length = dataLength * 8;
        SPITransaction.tx_buffer = data;

        spi_device_transmit(SPIHandle, &SPITransaction);
    }

}

void spi_write_command(display_t *dev, uint8_t cmd) {
    static uint8_t Byte = 0;
    Byte = cmd;
    gpio_set_level(dev->_RS, SPI_Command_Mode);
    spi_write_byte(dev->_SPIHandle, &Byte, 1);
}

void spi_write_data(display_t *dev, uint8_t *data, int16_t len) {
    gpio_set_level(dev->_RS, SPI_Data_Mode);
    spi_write_byte(dev->_SPIHandle, &data, len);
}

void spi_write_data_byte(display_t *dev, uint8_t *data) {
    static uint8_t Byte = 0;
    Byte = data;
    gpio_set_level(dev->_RS, SPI_Data_Mode);
    spi_write_byte(dev->_SPIHandle, &data, 1);
}

void delayMS(int ms) {
	int _ms = ms + (portTICK_PERIOD_MS - 1);
	TickType_t xTicksToDelay = _ms / portTICK_PERIOD_MS;
	vTaskDelay(xTicksToDelay);
}