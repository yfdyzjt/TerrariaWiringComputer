#ifndef DATA_H
#define DATA_H

#include <stdint.h>
#include <stdbool.h>

#define WIDTH 7
#define HIGHTH 6
#define CONNECTION 4

typedef struct
{
    int8_t x, y;
} Point;

typedef struct
{
    bool data[WIDTH][HIGHTH];
} Bitmap;

typedef struct Player Player;
struct Player
{
    Bitmap bitmap;
    int8_t x, y;
    bool color;
    void (*draw_name)();
    bool (*move)(Player *player, Player *competitor);
};

#endif
