#include "display_96_64.h"

int main()
{
    for (int y = 0; y < DISPLAY_SIZE_Y; y++)
        for (int x = 0; x < DISPLAY_SIZE_X; x++)
            draw_pixel(x, y, ((x & y) == 0) ? 1 : 0);

    display_refresh();
    return 0;
}
