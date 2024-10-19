#include "display_96_64.h"
#include "driver.h"

enum Direction
{
    U = 0,
    R = 1,
    D = 2,
    L = 3
};

typedef struct
{
    int x;
    int y;
    enum Direction dir;
} Ant;

unsigned char get_pixel(int x, int y)
{
    return (_display_buffer[x / 8 + y * 16] >> (x % 8)) & 1;
}

void set_pixel(int x, int y, unsigned char pixel)
{
    draw_pixel(x, y, pixel);
}

void direction_update(Ant *ant, unsigned char pixel_color)
{
    if (pixel_color == 0)
        ant->dir = (ant->dir + 1) % 4;
    else
        ant->dir = (ant->dir + 3) % 4;
}

void ant_move(Ant *ant)
{
    switch (ant->dir)
    {
    case U:
        ant->y -= 1;
        break;
    case R:
        ant->x -= 1;
        break;
    case D:
        ant->y += 1;
        break;
    case L:
        ant->x += 1;
        break;
    }
}

int main()
{
    Ant ant = {DISPLAY_SIZE_X / 2, DISPLAY_SIZE_Y / 2, U};

    unsigned char pixel;

    display_clear();
    while (ant.x >= 0 && ant.x < DISPLAY_SIZE_X && ant.y >= 0 && ant.y < DISPLAY_SIZE_Y)
    {
        pixel = get_pixel(ant.x, ant.y);
        direction_update(&ant, pixel);
        set_pixel(ant.x, ant.y, 1 - pixel);
        ant_move(&ant);
        display_refresh();
    }
    driver_off();
    return 0;
}