#include "output.h"
#include "display_96_64.h"

#include <stdint.h>

void string_draw(int32_t x, int32_t y, const char *str)
{
    draw_string(DISPLAY_SIZE_X + 1 - x, y, str);
}

void pixel_draw(int32_t x, int32_t y, uint8_t pixel)
{
    draw_pixel(DISPLAY_SIZE_X - 1 - x, y, pixel);
}

void draw_refresh()
{
    display_refresh();
}

void draw_clean()
{
    display_clear();
}
