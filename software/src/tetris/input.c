#include "six_way_motion_sensor.h"

#include "input.h"
#include "matrix.h"
#include "preview.h"
#include "data.h"
#include "score.h"

#define ROTATION_DELAY 10
#define DAS_DELAY 10
#define ARR_DELAY 2

unsigned short move_left_time;
unsigned short move_right_time;
unsigned short rotation_clockwise_time;

unsigned short last_input_time;

signed char is_hold;

signed char up();
signed char down();
signed char left();
signed char right();
signed char space();
signed char shot();

const signed char (*rotation_clockwise_input_func)() = up;
const signed char (*soft_drop_input_func)() = down;
const signed char (*move_left_input_func)() = left;
const signed char (*move_right_input_func)() = right;
const signed char (*hard_drop_input_func)() = space;
const signed char (*hold_input_func)() = shot;

signed char rotation(signed char dr)
{
    if (rotation_clockwise_time == 0)
        return matrix_tetromino_spin(dr);
    else if (rotation_clockwise_time >= ROTATION_DELAY)
    {
        rotation_clockwise_time = 0;
        return matrix_tetromino_spin(dr);
    }
    else
        return 0;
}

signed char rotation_clockwise()
{
    return rotation(+1);
}

signed char rotation_counterclockwise()
{
    return rotation(-1);
}

signed char move_left_right(unsigned short *time, signed char dx)
{
    if (*time == 0 || *time == DAS_DELAY)
        return matrix_tetromino_move(dx, +0);
    else if (*time >= DAS_DELAY + ARR_DELAY)
    {
        *time = DAS_DELAY;
        return matrix_tetromino_move(dx, +0);
    }
    return 0;
}

signed char move_left()
{
    return move_left_right(&move_left_time, +1);
}

signed char move_right()
{
    return move_left_right(&move_right_time, -1);
}

signed char soft_drop()
{
    score_soft_drop();
    return matrix_tetromino_move(+0, +1);
}

signed char hard_drop()
{
    signed char dy = matrix_tetromino_can_move_number();
    matrix_tetromino_move(+0, dy);
    score_hard_drop(dy);
    return matrix_tetromino_lock();
}

signed char hold()
{
    if (!is_hold)
    {
        is_hold = 1;
        return matrix_tetromino_hold();
    }
    else
        return 0;
}

signed char up()
{
    return six_motion_sensor_u();
}

signed char down()
{
    return six_motion_sensor_d();
}

signed char left()
{
    return six_motion_sensor_l();
}

signed char right()
{
    return six_motion_sensor_r();
}

signed char space()
{
    return six_motion_sensor_j();
}

signed char shot()
{
    return six_motion_sensor_s();
}

void map_input_to_update_time(signed char (*input_func)(), unsigned short *time)
{
    if (input_func())
        *time += time_gap;
    else
        *time = 0;
}

void last_input_update_time(signed char flag)
{
    if (!flag)
        last_input_time += time_gap;
    else
        last_input_time = 0;
}

signed char map_input_to_action(signed char (*input_func)(), signed char (*action_func)())
{
    if (input_func())
        return action_func();
    else
        return 0;
}

void input_update()
{
    signed char input_success_flag = 0;

    six_motion_sensor_refresh();

    input_success_flag |= map_input_to_action(rotation_clockwise_input_func, rotation_clockwise);
    input_success_flag |= map_input_to_action(move_left_input_func, move_left);
    input_success_flag |= map_input_to_action(move_right_input_func, move_right);
    input_success_flag |= map_input_to_action(soft_drop_input_func, soft_drop);
    input_success_flag |= map_input_to_action(hard_drop_input_func, hard_drop);
    input_success_flag |= map_input_to_action(hold_input_func, hold);

    last_input_update_time(input_success_flag);

    map_input_to_update_time(rotation_clockwise_input_func, &rotation_clockwise_time);
    map_input_to_update_time(move_left_input_func, &move_left_time);
    map_input_to_update_time(move_right_input_func, &move_right_time);
}
