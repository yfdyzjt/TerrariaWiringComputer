#ifndef GAME_H
#define GAME_H

#include <stdint.h>

#include "data.h"

void player_init(Player *player, void (*draw_name)(), bool (*move)(Player *player, Player *competitor), bool color);
bool player_insert(Player *player, Player *competitor);
bool player_is_connect(Player *player, Point *pos);

int32_t bitmap_insert(Bitmap *player, Bitmap *competitor, int32_t x);
bool bitmap_is_full(Bitmap *player, Bitmap *competitor);
bool bitmap_is_connect(Bitmap *bitmap, int32_t x, int32_t y);

#endif
