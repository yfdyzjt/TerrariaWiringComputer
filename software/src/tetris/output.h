#ifndef OUTPUT
#define OUTPUT

#include "tetromino.h"

void draw_ui();

void draw_grid_copy_up_down(signed char x, signed char from_y, signed char to_y, signed char w, signed char h);
void draw_tetromino_grid_up(signed char x, signed char y, TetroType type);
void draw_tetromino_grid_matrix(signed char mx, signed char my, unsigned short *grid, signed char pixel);
void draw_matrix(signed char x, signed char y, signed char w, signed char h, unsigned int *board);

void draw_score();
void draw_level();
void draw_lines();

void draw_refresh();
void draw_clear();

void interrupt();

#endif