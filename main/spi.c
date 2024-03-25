#include "spi.h"

#ifdef CONFIG_IDF_TARGET_ESP32
#define HOST    HSPI_HOST
#else
#define HOST    SPI2_HOST
#endif

void init_spi(int8_t GPIO_MOSI, int8_t GPIO_MISO, int8_t GPIO_SCLK, int8_t GPIO_QUADWP, int8_t GPIO_QUADHD) {
    spi_bus_config_t spi_cfg = {
        .mosi_io_num = GPIO_MOSI,
        .miso_io_num = GPIO_MISO,
        .sclk_io_num = GPIO_SCLK,
        .quadwp_io_num = GPIO_QUADWP,
        .quadhd_io_num = GPIO_QUADHD
    };

    spi_bus_initialize(HOST, &spi_cfg, 0);
}

void spi_add(spi_device_handle_t *handle, uint8_t CS) {
    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = SPI_MASTER_FREQ_8M,
        .queue_size = 7,
        .mode = 0,
        .flags = SPI_DEVICE_NO_DUMMY,
        .spics_io_num = CS
    };
    spi_bus_add_device(HOST, &devcfg, handle);
}

void spi_write_byte(spi_device_handle_t SPIHandle, const uint8_t *data, size_t dataLength) {
    spi_transaction_t SPITransaction;

    if(dataLength > 0) {
        memset(&SPITransaction, 0, sizeof(spi_transaction_t));
        SPITransaction.length = dataLength * 8;
        SPITransaction.tx_buffer = data;

        spi_device_transmit(SPIHandle, &SPITransaction);
    }

}

void delayMS(int ms) {
	int _ms = ms + (portTICK_PERIOD_MS - 1);
	TickType_t xTicksToDelay = _ms / portTICK_PERIOD_MS;
	vTaskDelay(xTicksToDelay);
}