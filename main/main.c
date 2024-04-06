#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "speed_sensor.h"
#include "st7565.h"
#include "spi.h"

#define UNIT_GPIO 32
#define NUM_OF_UNITS 2

void IRAM_ATTR unit_isr_handler(void *arg) {
    hall_sensor_t *sensor = (hall_sensor_t*)arg;
    sensor->_unit++;
    sensor->_unit %= NUM_OF_UNITS;
}

void start(display_t *dev, hall_sensor_t *sensor) {
     gpio_config_t unit_gpio = {
        .pin_bit_mask = (1ULL << UNIT_GPIO),
        .mode = GPIO_MODE_INPUT,
        .pull_down_en = GPIO_PULLDOWN_ENABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .intr_type = GPIO_INTR_POSEDGE
    };
    gpio_config(&unit_gpio);
    gpio_install_isr_service(1<<9);

    gpio_isr_handler_add(UNIT_GPIO, unit_isr_handler, (void*)sensor);
    }

void testdisplay(display_t *dev) {
    lcd_fill_buffer(dev);
    lcd_write_buffer(dev);
}

void draw_speed(display_t *dev, hall_sensor_t *sensor) {
    uint16_t old_cursor = dev->_cursor;
    float speed = 0;
    lcd_set_cursor(dev, 11*8, 7);
    switch(sensor->_unit) {
        case 0: // m/s
            speed = sensor->_speed;
            lcd_write_string(dev, " m/s");
            break;
        case 1: // km/h
            speed = sensor->_speed * 3.6;
            lcd_write_string(dev, "km/h");
            break;
        default:
        break;
    }
    sprintf(sensor->_speed_str, "%.1f", speed);
    lcd_set_cursor(dev, 10*8, 6);
    lcd_write_string(dev, sensor->_speed_str);
    lcd_write_char(dev, ' ');
    dev->_cursor = old_cursor;
}


// CS: 25 RESET: 26 A0: 27
void app_main(void)
{
    
    init_spi(12, -1, 14, -1, -1);
    display_t dev;
    hall_sensor_t sensor;
    start(&dev, &sensor);
    init_speed_sensor(&sensor, 33, 2100, 1, 2000000);
    // xTaskCreate(start, "start", 1024*6, &dev, 2, NULL);
    init_display(&dev, 128, 64, 25, 26, 27);
    delayMS(1000);
    
    lcd_write_buffer(&dev);

    while(1) {
        printf("speed: %.1f\n", sensor._speed);
        draw_speed(&dev, &sensor);
        lcd_write_buffer(&dev);
        delayMS(500);
    }
}

