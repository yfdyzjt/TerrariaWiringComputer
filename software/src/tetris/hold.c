#include "hold.h"
#include "tetromino.h"
#include "output.h"

TetroType data = TETRO_NIL;

void hold_init()
{
    data = TETRO_NIL;
}

TetroType hold_get_set(TetroType type)
{
    draw_tetromino_grid_up(HOLD_POS_X, HOLD_POS_Y, type);

    TetroType value = data;
    data = type;
    return value;
}
