#include <stdbool.h>
#include <stdint.h>

#include "five_way_motion_sensor.h"
#include "random_number_generator.h"

#include "input.h"
#include "data.h"

void key_refresh()
{
    motion_sensor_refresh();
}

bool key_j()
{
    return motion_sensor_j();
}

bool key_l()
{
    return motion_sensor_l();
}

bool key_r()
{
    return motion_sensor_r();
}

uint32_t random_number_get()
{
    return rand.u32;
}

void key_wait()
{
    while (true)
    {
        motion_sensor_refresh();
        if (motion_sensor_j())
            return;
    }
}
