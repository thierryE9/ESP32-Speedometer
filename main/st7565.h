#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <driver/gpio.h>
#include <driver/spi_master.h>
#include <fonts.h>

#ifndef ST7565_H
#define ST7565_H

static const int SPI_Command_Mode = 0;
static const int SPI_Data_Mode = 1;

typedef struct {
    uint16_t _width;
    uint16_t _height;
    uint16_t _A;
    uint16_t _numOfBytes;
    spi_device_handle_t _SPIHandle;
    uint8_t *_buffer;
    uint16_t _cursor; // keep track of cursor for typing text
} display_t;

void init_display(display_t *dev, int width, int height, uint8_t GPIO_CS, uint8_t GPIO_RESET, uint8_t GPIO_A0);
void lcd_draw_pixel(display_t *dev, uint16_t x, uint16_t y, uint8_t color);
void lcd_write_buffer(display_t *dev);
void lcd_fill_buffer(display_t *dev);
void lcd_write_char(display_t *dev, char letter);
void lcd_write_string(display_t *dev, char *str);
// x: column to 127. y: page
void lcd_set_cursor(display_t *dev, uint8_t x, uint8_t y); 
void spi_write_command(display_t *dev, uint8_t cmd);
void spi_write_data(display_t *dev, uint8_t data, int16_t len);
void spi_write_data_byte(display_t *dev, uint8_t data);

#endif