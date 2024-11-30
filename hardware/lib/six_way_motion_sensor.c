#include "six_way_motion_sensor.h"

volatile const unsigned char _six_motion_sensor_buffer __attribute__((section("six_way_motion_sensor.h")));
unsigned char _six_motion_sensor_cache = 0;

void six_motion_sensor_refresh()
{
    _six_motion_sensor_cache = _six_motion_sensor_buffer;
}

char six_motion_sensor_u()
{
    return (_six_motion_sensor_cache & SIX_MOTION_SENSOR_U) && 1;
}

char six_motion_sensor_d()
{
    return (_six_motion_sensor_cache & SIX_MOTION_SENSOR_D) && 1;
}

char six_motion_sensor_r()
{
    return (_six_motion_sensor_cache & SIX_MOTION_SENSOR_R) && 1;
}

char six_motion_sensor_l()
{
    return (_six_motion_sensor_cache & SIX_MOTION_SENSOR_L) && 1;
}

char six_motion_sensor_j()
{
    return (_six_motion_sensor_cache & SIX_MOTION_SENSOR_J) && 1;
}

char six_motion_sensor_s()
{
    return (_six_motion_sensor_cache & SIX_MOTION_SENSOR_S) && 1;
}
