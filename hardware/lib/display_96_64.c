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

void draw_pixel(int posX, int posY, unsigned char pixel)
{
    unsigned char *addr = &_display_buffer[posX / 8 + posY * 16];
    int offsetX = posX % 8;
    *addr = pixel ? *addr | (1 << offsetX) : *addr & ~(1 << offsetX);
}

void draw_grid(int posX, int posY, int sizeX, int sizeY, unsigned char *grid)
{
    unsigned char *addr = &_display_buffer[posX / 8 + posY * 16];

    int i;

    int high, low;
    unsigned short mask, date;

    low = posX % 8;
    high = low + sizeX;
    mask = (unsigned short)(((1 << high) - 1) ^ ((1 << low) - 1));

    for (i = 0; i < sizeY; i++)
    {
        if (i + posY < 0 && i + posY >= DISPLAY_SIZE_Y)
            continue;
        date = (unsigned short)((grid[i] << low) & mask);

        if (posX >= 0 && posX < DISPLAY_SIZE_X)
            *addr = (*addr & ~*((unsigned char *)&mask)) ^ *((unsigned char *)&date);
        if (high >= 8 && posX + 8 >= 0 && posX + 8 < DISPLAY_SIZE_X)
            *(addr + 1) = (*(addr + 1) & ~*((unsigned char *)&mask + 1)) ^ *((unsigned char *)&date + 1);
        addr += 16;
    }
}

void draw_char(int posX, int posY, char c)
{
    draw_grid(posX, posY, 5, 7, (unsigned char *)g57Ascii[c - 32]);
}

void draw_string(int posX, int posY, char *s)
{
    int i = 0;
    while (s[i] != '\0')
    {
        posX -= 6;
        draw_grid(posX, posY, 6, 7, (unsigned char *)g57Ascii[s[i++] - 32]);
    }
}
