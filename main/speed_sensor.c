#include "speed_sensor.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include "spi.h"

void init_speed_sensor(hall_sensor_t *sensor, uint8_t gpio, int perimiter_mm, int num_of_magnets, int stopped_threshold_us) {
    gpio_config_t sen_gpio = {
        .pin_bit_mask = (1ULL << gpio),
        .mode = GPIO_MODE_INPUT,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .intr_type = GPIO_INTR_ANYEDGE
    };
    gpio_config(&sen_gpio);

    sensor->_GPIO_NUM = gpio;
    sensor->_last_trigger_tick = 0;
    sensor->_speed = 0;
    sensor->_perimiter_mm = perimiter_mm;
    sensor->_num_of_magnets = num_of_magnets;
    sensor->_is_moving = 0;
    sensor->_STOPPED_THRESHOLD_uS = stopped_threshold_us;

    gpio_install_isr_service(1<<9);
    gpio_isr_handler_add(sensor->_GPIO_NUM, speed_isr_handler, (void*)sensor);
}

void IRAM_ATTR speed_isr_handler(void *arg) {
    hall_sensor_t *sensor = (hall_sensor_t*)arg;
    int64_t current_tick = esp_timer_get_time();
    int64_t time = current_tick - sensor->_last_trigger_tick;

    if(sensor->_is_moving) {
        sensor->_speed = (int)((sensor->_perimiter_mm / sensor->_num_of_magnets) / time*1000); // in mm/ms same as m/s
    }
    sensor->_last_trigger_tick = current_tick;
    sensor->_is_moving = 1;
    
}

void check_if_moving_task(hall_sensor_t *sensor) {
    while(1) {
        delayMS(1000);
        if((esp_timer_get_time() - sensor->_last_trigger_tick) > sensor->_STOPPED_THRESHOLD_uS) {
        sensor->_speed = 0;
        sensor->_is_moving = 0;
        }
    }
    
}