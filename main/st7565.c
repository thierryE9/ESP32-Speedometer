#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <driver/spi_master.h>
#include <driver/gpio.h>
#include "esp_log.h"

#include "st7565.h"
#include "spi.h"


void init_display(display_t *dev, int width, int height) {
    init_spi(dev);
    
	gpio_reset_pin( GPIO_RESET );
	gpio_set_direction( GPIO_RESET, GPIO_MODE_OUTPUT );
	gpio_set_level( GPIO_RESET, 1 );
	delayMS(50);
	gpio_set_level( GPIO_RESET, 0 );
	delayMS(50);
	gpio_set_level( GPIO_RESET, 1 );
	delayMS(50);

    gpio_reset_pin( GPIO_RS );
	gpio_set_direction( GPIO_RS, GPIO_MODE_OUTPUT );
	gpio_set_level( GPIO_RS, 0 );
	
	gpio_reset_pin( GPIO_CS_LCD );
	gpio_set_direction( GPIO_CS_LCD, GPIO_MODE_OUTPUT );
	gpio_set_level( GPIO_CS_LCD, 0 );

    dev->_width = width;
    dev->_height = height;
    dev->_RS = GPIO_RS;
	dev->_numOfBytes = width*(height/8);
	uint8_t *buffer = (uint8_t*)malloc(dev->_numOfBytes);
	dev->_buffer = buffer;

    spi_write_command(dev, 0xe2); // system reset
	spi_write_command(dev, 0x40); // set LCD start line to 0
	spi_write_command(dev, 0xa0); // set SEG direction (A1 to flip horizontal)
	spi_write_command(dev, 0xc8); // set COM direction (C0 to flip vert)
	spi_write_command(dev, 0xa2); // set LCD bias mode 1/9
	spi_write_command(dev, 0x2c); // set boost on
	spi_write_command(dev, 0x2e); // set voltage regulator on
	spi_write_command(dev, 0x2f); // Voltage follower on
	spi_write_command(dev, 0xf8); // set booster ratio to
	spi_write_command(dev, 0x00); // 4x
	spi_write_command(dev, 0x23); // set resistor ratio = 4
	spi_write_command(dev, 0x81);
	spi_write_command(dev, 0x3F); // set contrast = 40
	spi_write_command(dev, 0xac); // set static indicator off
	spi_write_command(dev, 0x00);
	spi_write_command(dev, 0xa6); // disable inverse
	spi_write_command(dev, 0xaf); // enable display
	delayMS(100);
	spi_write_command(dev, 0xa5); // display all points
	delayMS(200);
	spi_write_command(dev, 0xa4); // normal displayW

	// spi_write_data(dev, 0x0C, 1);
	// spi_write_data(dev, 0x01, 1);
	// spi_write_data(dev, 0x09, 1);
	// spi_write_data(dev, 0x00, 1);
	// spi_write_data(dev, 0x01, 1);
	// spi_write_data(dev, 0x01, 1);
	// spi_write_data(dev, 0x0A, 1);
	// spi_write_data(dev, 0x0A, 1);
	// spi_write_data(dev, 0x0C, 1);
	// spi_write_data(dev, 0x01, 1);
	// spi_write_data(dev, 0b00000000, 1);
	// spi_write_data(dev, 0b01101101, 1);

}

void lcd_draw_pixel(display_t *dev, uint16_t x, uint16_t y, uint8_t color) {
	if(color>0){
		dev->_buffer[x + (y/8)*dev->_width] |= 1;
	} else{
		dev->_buffer[x + (y/8)*dev->_width] &= 0;
	}
}

void lcd_write_buffer(display_t *dev) {
	uint16_t x, y;
	for(y=0; y<8; y++) {
		spi_write_command(dev, 0xb0 | y);
		spi_write_command(dev, 0x10);
		for(x=0; x<dev->_width; x++) {
			spi_write_data_byte(dev, dev->_buffer[x+y*128]);
		}
	}
}

void lcd_fill_buffer(display_t *dev) {
	for(int x=0; x<dev->_numOfBytes; x++) {
		dev->_buffer[x] = 0xff;
	}
}