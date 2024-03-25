#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <driver/spi_master.h>
#include <driver/gpio.h>
#include "esp_log.h"
#include <string.h>

#ifndef SPI_H
#define SPI_H


#define SPI_FREQUENCY SPI_MASTER_FREQ_8M

void init_spi(int8_t GPIO_MOSI, int8_t GPIO_MISO, int8_t GPIO_SCLK, int8_t GPIO_QUADWP, int8_t GPIO_QUADHD);
void spi_add(spi_device_handle_t *handle, uint8_t CS);
void spi_write_byte(spi_device_handle_t SPIHandle, const uint8_t *data, size_t dataLength);
void delayMS(int ms);


#endif
