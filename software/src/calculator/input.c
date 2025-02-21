#include "input.h"
#include "output.h"
#include "error.h"
#include "key.h"

#include "five_way_motion_sensor.h"

#include <stdint.h>

const KeyType key_pos[KEY_NUMBER_X][KEY_NUMBER_Y] = {
    {KEY_BACK, KEY_CE, KEY_C, KEY_EQ},
    {KEY_DIV, KEY_MUL, KEY_SUB, KEY_ADD},
    {KEY_9, KEY_6, KEY_3, KEY_DOT},
    {KEY_8, KEY_5, KEY_2, KEY_0},
    {KEY_7, KEY_4, KEY_1, KEY_NEG},
    {KEY_MR, KEY_RECIP, KEY_SQ, KEY_SQRT},
    {KEY_MC, KEY_MADD, KEY_MSUB, KEY_MS},
};

static int8_t cursor_x, cursor_y;

static ErrorType cursor_press()
{
    KeyType key = key_pos[cursor_x][cursor_y];
    return key_press[key](key);
}

static void cursor_move(int8_t dx, int8_t dy)
{
    if (cursor_x + dx >= 0 && cursor_x + dx < KEY_NUMBER_X &&
        cursor_y + dy >= 0 && cursor_y + dy < KEY_NUMBER_Y)
    {
        cursor_erase(cursor_x, cursor_y);
        cursor_x += dx;
        cursor_y += dy;
        cursor_draw(cursor_x, cursor_y);
    }
}

ErrorType cursor_update()
{
    ErrorType error = ERROR_NONE;

    motion_sensor_refresh();
    if (motion_sensor_j())
        error = cursor_press();
    else if (motion_sensor_l())
        cursor_move(+1, +0);
    else if (motion_sensor_d())
        cursor_move(+0, +1);
    else if (motion_sensor_r())
        cursor_move(-1, +0);
    else if (motion_sensor_u())
        cursor_move(+0, -1);

    return error;
}
