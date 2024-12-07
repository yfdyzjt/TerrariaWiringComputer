#include "display_96_64.h"

#define DRAW_SIZE_X 81
#define DRAW_SIZE_Y 81

int main()
{
    int draw, nx, ny;

    for (int y = 0; y < DRAW_SIZE_Y; y++)
        for (int x = 0; x < DRAW_SIZE_X; x++)
        {
            draw = 1;

            for (nx = x, ny = y; nx > 0 || ny > 0; nx /= 3, ny /= 3)
                if (nx % 3 == 1 && ny % 3 == 1)
                {
                    draw = 0;
                    break;
                }

            draw_pixel(x, y, draw);
        }

    display_refresh();
    return 0;
}
