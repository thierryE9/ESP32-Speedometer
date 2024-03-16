#include <stdio.h>
#include "freertos/FreeRTOS.h"

#include "st7565.h"
#include "spi.h"

void app_main(void)
{
    display_t dev;
    init_display(&dev, 128, 64);
}
