#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <driver/spi_master.h>
#include <driver/gpio.h>
#include "esp_log.h"

#include "st7565.h"
#include "spi.h"


void init_display(display_t *dev, int width, int height) {
    init_spi(dev);
    gpio_set_direction(GPIO_RESET, GPIO_MODE_OUTPUT);
    gpio_set_direction(dev->_RS, GPIO_MODE_OUTPUT);
    gpio_set_direction( GPIO_CS_LCD, GPIO_MODE_OUTPUT);

    dev->_width = width;
    dev->_height = height;
    dev->_RS = GPIO_RS;

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
	spi_write_command(dev, 0x28); // set contrast = 40
	spi_write_command(dev, 0xac); // set static indicator off
	spi_write_command(dev, 0x00);
	spi_write_command(dev, 0xa6); // disable inverse
	spi_write_command(dev, 0xaf); // enable display
	delayMS(100);
	spi_write_command(dev, 0xa5); // display all points
	delayMS(200);
	spi_write_command(dev, 0xa4); // normal display

}

