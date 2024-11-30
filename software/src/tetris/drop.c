#include "drop.h"
#include "matrix.h"
#include "input.h"
#include "data.h"
#include "helper.h"

#define LOCK_DELAY 30

#define SPEED_CURVE_LEVEL_LOW 13
#define SPEED_CURVE_LEVEL_HIGH 19

signed char in_touch_floor = 0;

unsigned short drop_time = 0;

const unsigned char speed_curve_low[SPEED_CURVE_LEVEL_LOW] = {
    60, 48, 37, 28, 21, 16, 11, 8, 6, 4, 3, 2, 1};
const unsigned char speed_curve_high[SPEED_CURVE_LEVEL_HIGH - SPEED_CURVE_LEVEL_LOW] = {
    1, 2, 4, 7, 11, 20};

signed char is_touch_floor()
{
    return matrix_tetromino_can_move_number() == 0;
}

void drop_check()
{
    if (!is_touch_floor())
    {
        if (level <= SPEED_CURVE_LEVEL_LOW)
        {
            if (drop_time >= speed_curve_low[level - 1])
            {
                matrix_tetromino_move(+0, +1);
                drop_time = 0;
            }

            drop_time += time_gap;
        }
        else if (time_gap >= 1)
        {
            unsigned char number = time_gap * speed_curve_high[min(SPEED_CURVE_LEVEL_HIGH, level) - SPEED_CURVE_LEVEL_LOW - 1];
            for (unsigned char i = 0; i < number; i++)
                matrix_tetromino_move(+0, +1);
        }
    }
}

void lock_check()
{
    if (is_touch_floor())
    {
        if (!in_touch_floor)
        {
            in_touch_floor = 1;
            last_input_time = 0;
        }
        else if (last_input_time >= LOCK_DELAY)
        {
            in_touch_floor = 0;
            matrix_tetromino_lock();
        }
    }
    else
        in_touch_floor = 0;
}

void drop_update()
{
    drop_check();
    lock_check();
}
