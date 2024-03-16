#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <driver/gpio.h>
#include <driver/spi_master.h>

#ifndef ST7565_H
#define ST7565_H

#define GPIO_RESET 26
#define GPIO_RS 27

static const int SPI_Command_Mode = 0;
static const int SPI_Data_Mode = 1;

typedef struct {
    uint16_t _width;
    uint16_t _height;
    uint16_t _RS;
    spi_device_handle_t _SPIHandle;
} display_t;

void init_display(display_t *dev, int width, int height);

#endif