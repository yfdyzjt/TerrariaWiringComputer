#include "display_96_64.h"

#define MAX_SIZE_Y DISPLAY_SIZE_Y
#define MAX_SIZE_X (MAX_SIZE_Y * 2)

unsigned char statue[2][MAX_SIZE_X / 8];

unsigned char get_pixel(int x, int y)
{
    if (x >= 0 && x < MAX_SIZE_X)
        return (statue[y & 1][x / 8] >> (x % 8)) & 1;
    else
        return 0;
}

void set_pixel(int x, int y, unsigned char pixel)
{
    if (x >= 0 && x < MAX_SIZE_X)
    {
        unsigned char *addr = &statue[y & 1][x / 8];
        int offsetX = x % 8;
        *addr = pixel ? *addr | (1 << offsetX) : *addr & ~(1 << offsetX);
    }

    draw_pixel(x - (MAX_SIZE_X - DISPLAY_SIZE_X) / 2, y, pixel);
}

void wolfram_automaton(int rule)
{
    unsigned char pattern, next_state;

    set_pixel(MAX_SIZE_X / 2, 0, 1);

    for (int y = 1; y < MAX_SIZE_Y; y++)
    {
        for (int x = 0; x < MAX_SIZE_X; x++)
        {
            pattern = get_pixel(x + 1, y - 1) << 2 |
                      get_pixel(x + 0, y - 1) << 1 |
                      get_pixel(x - 1, y - 1) << 0;

            next_state = (rule >> pattern) & 1;

            set_pixel(x, y, next_state);
        }

        display_refresh();
    }
}

int main()
{
    int rule = 30;

    // rule 30
    // rule 90
    // rule 110

    wolfram_automaton(rule);
    return 0;
}
