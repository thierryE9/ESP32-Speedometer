#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "st7565.h"
#include "spi.h"

void start(display_t *dev) {
    init_display(dev, 128, 64);
    vTaskDelete(NULL);
    }

void testdisply(display_t *dev) {
    // for(uint16_t y=0; y<dev->_height; y++) {
    //     for(uint16_t x=0; x<dev->_width; x++) {
    //         lcd_draw_pixel(dev, x, y, 1);
    //     }
    // }
    lcd_fill_buffer(dev);
    lcd_write_buffer(dev);
}

void app_main(void)
{
    display_t dev;
    xTaskCreate(start, "start", 1024*6, &dev, 2, NULL);
    delayMS(1000);
    testdisply(&dev);

    while(1) {
        delayMS(500);
    }
}
