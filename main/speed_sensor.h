#include "esp_attr.h"
#include <stdint.h>

#ifndef SPEED_SENSOR_H
#define SPEED_SENSOR_H

typedef struct {
    uint8_t _GPIO_NUM;
    int _speed; // in mm/ms same as m/s
    int64_t _last_trigger_tick;
    int _perimiter_mm;
    int _num_of_magnets;
    uint8_t _is_moving;
    int64_t _STOPPED_THRESHOLD_uS;
} hall_sensor_t;

void init_speed_sensor(hall_sensor_t *sensor, uint8_t gpio, int perimiter_mm, int num_of_sensors, int stopped_threshold_us);

void IRAM_ATTR speed_isr_handler(void *arg);

void check_if_moving_task(hall_sensor_t *sensor);

#endif