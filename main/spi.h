#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <driver/spi_master.h>
#include <driver/gpio.h>
#include "esp_log.h"
#include <string.h>

#include "st7565.h"

#ifndef SPI_H
#define SPI_H

#define GPIO_MOSI 12
#define GPIO_MISO -1
#define GPIO_SCLK 14
#define GPIO_QUADWP -1
#define GPIO_QUADHD -1
#define GPIO_CS_LCD 25

#define SPI_FREQUENCY SPI_MASTER_FREQ_8M

void init_spi(display_t *dev);
void spi_write_byte(spi_device_handle_t SPIHandle, const uint8_t *data, size_t dataLength);
void spi_write_command(display_t *dev, uint8_t cmd);
void spi_write_data(display_t *dev, uint8_t data, int16_t len);
void spi_write_data_byte(display_t *dev, uint8_t data);
void delayMS(int ms);


#endif
