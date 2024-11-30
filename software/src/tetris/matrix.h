#ifndef MATRIX
#define MATRIX

#include "tetromino.h"

#define MATRIX_W 10
#define MATRIX_H 20

#define MATRIX_POS_X 17
#define MATRIX_POS_Y 2

#define MATRIX_VISUAL_UPPER 11
#define MATRIX_VISUAL_LOWER 31

void matrix_init();
void matrix_start();

signed char matrix_tetromino_move(signed char dx, signed char dy);
signed char matrix_tetromino_spin(signed char dr);
signed char matrix_tetromino_can_move_number();
signed char matrix_tetromino_lock();
signed char matrix_tetromino_hold();
signed char matrix_tetromino_spawn(TetroType type);

#endif
