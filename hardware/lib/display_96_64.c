#include "display_96_64.h"
#include "ascii_5_7.h"

unsigned char _display_buffer[1024] __attribute__((section("display_96_64.h")));
volatile unsigned char *_display_ctrl = &_display_buffer[1023];

void display_clear()
{
    *_display_ctrl = DISPLAY_CLC;
}

void display_refresh()
{
    *_display_ctrl = DISPLAY_REF;
}

void draw_pixel(int x, int y, unsigned char pixel)
{
    if (x >= 0 && x < DISPLAY_SIZE_X && y >= 0 && y < DISPLAY_SIZE_Y)
    {
        unsigned char *addr = &_display_buffer[x / 8 + y * 16];
        int offsetX = x % 8;
        *addr = pixel ? *addr | (1 << offsetX) : *addr & ~(1 << offsetX);
    }
}

int solid_line_one_func(int step, int x, int y)
{
    return 1;
}

int solid_line_zero_func(int step, int x, int y)
{
    return 0;
}

int dotted_line_func(int step, int x, int y)
{
    return (step & 1);
}

void draw_line(int x0, int y0, int x1, int y1, line_evolution_func evolve)
{
    int dx = (x1 - x0 < 0) ? -(x1 - x0) : (x1 - x0);
    int dy = (y1 - y0 < 0) ? -(y1 - y0) : (y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;
    int step = 0;

    while (1)
    {
        draw_pixel(x0, y0, evolve(step, x0, y0));

        if (x0 == x1 && y0 == y1)
            break;

        int e2 = err * 2;

        if (e2 > -dy)
        {
            err -= dy;
            x0 += sx;
        }

        if (e2 < dx)
        {
            err += dx;
            y0 += sy;
        }

        step++;
    }
}

void draw_rect(int x, int y, int w, int h, line_evolution_func evolve)
{
    draw_line(x, y, x + w, y, evolve);
    draw_line(x, y + h, x + w, y + h, evolve);
    draw_line(x, y, x, y + h, evolve);
    draw_line(x + w, y, x + w, y + h, evolve);
}

void draw_grid_char(int x, int y, int w, int h, const unsigned char *grid)
{
    unsigned char *addr = &_display_buffer[x / 8 + y * 16];

    int low = x % 8;
    int high = low + w;

    union
    {
        unsigned short s;
        unsigned char c[2];
    } u_data, u_mask;

    u_mask.s = ((1 << high) - 1) ^ ((1 << low) - 1);

    for (int i = 0; i < h; i++, addr += 16)
    {
        if (i + y < 0 || i + y >= DISPLAY_SIZE_Y)
            continue;

        u_data.s = (unsigned short)((grid[i] << low) & u_mask.s);

        if (x >= 0 && x < DISPLAY_SIZE_X)
            *addr = (*addr & ~u_mask.c[0]) ^ u_data.c[0];
        if (high >= 8 && x + 8 >= 0 && x + 8 < DISPLAY_SIZE_X)
            *(addr + 1) = (*(addr + 1) & ~u_mask.c[1]) ^ u_data.c[1];
    }
}

void draw_grid_short(int x, int y, int w, int h, const unsigned short *grid)
{
    unsigned short *addr = (unsigned short *)&_display_buffer[(x / 16) * 2 + y * 16];

    int low = x % 16;
    int high = low + w;

    union
    {
        unsigned int i;
        unsigned short s[2];
    } u_data, u_mask;

    u_mask.i = ((1 << high) - 1) ^ ((1 << low) - 1);

    for (int i = 0; i < h; i++, addr += 8)
    {
        if (i + y < 0 || i + y >= DISPLAY_SIZE_Y)
            continue;

        u_data.i = (unsigned int)((grid[i] << low) & u_mask.i);

        if (x >= 0 && x < DISPLAY_SIZE_X)
            *addr = (*addr & ~u_mask.s[0]) ^ u_data.s[0];
        if (high >= 16 && x + 16 >= 0 && x + 16 < DISPLAY_SIZE_X)
            *(addr + 1) = (*(addr + 1) & ~u_mask.s[1]) ^ u_data.s[1];
    }
}

void draw_grid_int(int x, int y, int w, int h, const unsigned int *grid)
{
    unsigned int *addr = (unsigned int *)&_display_buffer[(x / 32) * 4 + y * 16];

    int low = x % 32;
    int high = low + w;

    union
    {
        unsigned long long l;
        unsigned int i[2];
    } u_data, u_mask;

    u_mask.l = ((1ULL << high) - 1) ^ ((1ULL << low) - 1);

    for (int i = 0; i < h; i++, addr += 4)
    {
        if (i + y < 0 || i + y >= DISPLAY_SIZE_Y)
            continue;

        u_data.l = ((unsigned long long)grid[i] << low) & u_mask.l;

        if (x >= 0 && x < DISPLAY_SIZE_X)
            *addr = (*addr & ~u_mask.i[0]) ^ u_data.i[0];
        if (high >= 32 && x + 32 >= 0 && x + 32 < DISPLAY_SIZE_X)
            *(addr + 1) = (*(addr + 1) & ~u_mask.i[1]) ^ u_data.i[1];
    }
}

void draw_char(int x, int y, char c)
{
    draw_grid_char(x, y, 5, 7, (unsigned char *)g57Ascii[c - 32]);
}

void draw_string(int x, int y, char *s)
{
    int i = 0;
    while (s[i] != '\0')
    {
        x -= 6;
        draw_grid_char(x, y, 6, 7, (unsigned char *)g57Ascii[s[i++] - 32]);
    }
}
