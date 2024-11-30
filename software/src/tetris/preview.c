#include "preview.h"
#include "bag.h"
#include "tetromino.h"
#include "output.h"

unsigned char current_index;
TetroType preview_data[PREVIEW_LENGTH];

TetroType preview_data_get_set(TetroType new_value)
{
    TetroType old_value = preview_data[current_index];
    preview_data[current_index] = new_value;
    current_index = (current_index + 1) % PREVIEW_LENGTH;
    return old_value;
}

TetroType preview_get()
{
    TetroType new_value = bag_get();
    TetroType ret_value = preview_data_get_set(new_value);

    draw_grid_copy_up_down(PREVIEW_POS_X,
                           PREVIEW_POS_Y + TETROMINO_UP_H * TETROMINO_GRID_H + 2,
                           PREVIEW_POS_Y,
                           TETROMINO_UP_W * TETROMINO_GRID_W,
                           (TETROMINO_UP_H * TETROMINO_GRID_H + 2) * (PREVIEW_LENGTH - 1) - 2);

    draw_tetromino_grid_up(PREVIEW_POS_X,
                           PREVIEW_POS_Y + (TETROMINO_UP_H * TETROMINO_GRID_H + 2) * (PREVIEW_LENGTH - 1),
                           new_value);

    return ret_value;
}

void preview_start()
{
    for (unsigned char i = 0; i < PREVIEW_LENGTH; i++)
        preview_get();
}