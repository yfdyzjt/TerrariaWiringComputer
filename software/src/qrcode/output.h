#ifndef OUTPUT
#define OUTPUT

#include <stdint.h>

void string_draw(int32_t x, int32_t y, const char *str);
void pixel_draw(int32_t x, int32_t y, uint8_t pixel);
void draw_refresh();
void draw_clean();

#endif
