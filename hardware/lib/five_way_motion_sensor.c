#include "five_way_motion_sensor.h"

volatile const unsigned char _motion_sensor_buffer __attribute__((section("five_way_motion_sensor.h")));
unsigned char _motion_sensor_cache = 0;

void motion_sensor_refresh()
{
    _motion_sensor_cache = _motion_sensor_buffer;
}

char motion_sensor_u()
{
    return (_motion_sensor_cache & MOTION_SENSOR_U) && 1;
}

char motion_sensor_d()
{
    return (_motion_sensor_cache & MOTION_SENSOR_D) && 1;
}

char motion_sensor_r()
{
    return (_motion_sensor_cache & MOTION_SENSOR_R) && 1;
}

char motion_sensor_l()
{
    return (_motion_sensor_cache & MOTION_SENSOR_L) && 1;
}

char motion_sensor_j()
{
    return (_motion_sensor_cache & MOTION_SENSOR_J) && 1;
}
