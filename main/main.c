#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "st7565.h"
#include "spi.h"

void start(display_t *dev) {
    init_display(&dev, 128, 64);
    }

void app_main(void)
{
    display_t dev;
    xTaskCreate(start, "start", 1024*6, NULL, 2, NULL);

    while(1) {
        delayMS(500);
    }
}
