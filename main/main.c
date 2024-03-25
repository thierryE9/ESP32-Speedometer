#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "st7565.h"
#include "spi.h"



void start(display_t *dev) {
    // init_spi();
    // init_display(dev, 128, 64);
    vTaskDelete(NULL);
    }

void testdisplay(display_t *dev) {
    lcd_fill_buffer(dev);
    lcd_write_buffer(dev);
}

// CS: 25 RESET: 26 A0: 27
void app_main(void)
{
    // 
    init_spi(12, -1, 14, -1, -1);
    display_t dev;
    // xTaskCreate(start, "start", 1024*6, &dev, 2, NULL);
    init_display(&dev, 128, 64, 25, 26, 27);
    delayMS(1000);
    lcd_set_cursor(&dev, 58, 3);
    lcd_write_string(&dev, "Hello World!");
    lcd_write_buffer(&dev);

    while(1) {
        delayMS(500);
    }
}
