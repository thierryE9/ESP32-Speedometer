#include "speed_sensor.h"
#include "driver/gpio.h"
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
    sensor->_current_trigger_tick = 3000000;
    sensor->_speed = 0.0;
    sensor->_perimiter_mm = perimiter_mm;
    sensor->_num_of_magnets = num_of_magnets;
    sensor->_is_moving = 0;
    sensor->_STOPPED_THRESHOLD_uS = stopped_threshold_us;
    sensor->_unit = 0;

    gpio_isr_handler_add(sensor->_GPIO_NUM, speed_isr_handler, (void*)sensor);

    const esp_timer_create_args_t timer_cfg = {
        .callback = &check_if_moving_timer,
        .arg = (void*)sensor, // Pass the address of arg1
        .name = "my_timer"
    };

    // Create the timer
    esp_timer_handle_t timer_handle;
    esp_timer_create(&timer_cfg, &timer_handle);
    // Start the timer
    esp_timer_start_periodic(timer_handle, 1000);
    sensor->_timer_handle = timer_handle;
}

void IRAM_ATTR speed_isr_handler(void *arg) {
    hall_sensor_t *sensor = (hall_sensor_t*)arg;
    sensor->_current_trigger_tick = esp_timer_get_time();
    sensor->_has_pulsed = 1;    
}

void check_if_moving_timer(void *arg) {
    hall_sensor_t *sensor = (hall_sensor_t*)arg;
    int64_t current_time = esp_timer_get_time();
    int64_t elapsed_time = current_time - sensor->_last_trigger_tick;
    if((elapsed_time > sensor->_STOPPED_THRESHOLD_uS) && !sensor->_has_pulsed) {
        sensor->_speed = 0;
        // sensor->_is_moving = 0;
        return;
    } else if((elapsed_time > sensor->_STOPPED_THRESHOLD_uS) && sensor->_has_pulsed) {
        sensor->_speed = 0;
        // sensor->_is_moving = 0;
        sensor->_has_pulsed = 0;
        sensor->_last_trigger_tick = sensor->_current_trigger_tick;
    }
    if(sensor->_has_pulsed) {
        sensor->_speed = (float)((sensor->_perimiter_mm/sensor->_num_of_magnets))/((float)(sensor->_current_trigger_tick - sensor->_last_trigger_tick))*1000;
        sensor->_last_trigger_tick = sensor->_current_trigger_tick;
        sensor->_has_pulsed = 0;
    }

    
}

