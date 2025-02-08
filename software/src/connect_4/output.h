#ifndef OUTPUT_H
#define OUTPUT_H

#include <stdint.h>

#include "data.h"

void draw_nobody_text();
void draw_player_text();
void draw_computer_text();
void draw_win_text();

void draw_win_piece(Point *pos);
void draw_player_erase_piece(Player *player);
void draw_player_piece(Player *player);
void draw_init();

#endif