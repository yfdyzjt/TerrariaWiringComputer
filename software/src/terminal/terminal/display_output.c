#include "display_output.h"
#include "display_96_64.h"

#include <stdint.h>

#define CHARACTER_WIDTH 5
#define CHARACTER_HEIGHT 7

#define CURSOR_STEP_X (CHARACTER_WIDTH + 1)
#define CURSOR_STEP_Y (CHARACTER_HEIGHT + 1)

static int8_t cursor_x, cursor_y;

static void cursor_draw()
{
    draw_char(DISPLAY_SIZE_X - CHARACTER_WIDTH - cursor_x, cursor_y, '|');
    display_refresh();
}

static void cursor_erase()
{
    draw_char(DISPLAY_SIZE_X - CHARACTER_WIDTH - cursor_x, cursor_y, ' ');
}

static void cursor_draw_char(char c)
{
    draw_char(DISPLAY_SIZE_X - CHARACTER_WIDTH - cursor_x, cursor_y, c);
    cursor_x += CURSOR_STEP_X;
    cursor_draw();
    if (cursor_x >= DISPLAY_SIZE_X)
        display_output_nextline();
    if (cursor_y >= DISPLAY_SIZE_Y)
        display_output_nextpage();
}

static void display_output_previousline()
{
    cursor_erase();
    cursor_x = DISPLAY_SIZE_X - CHARACTER_WIDTH;
    cursor_y -= CURSOR_STEP_Y;
    cursor_draw();
}

void display_output_backspace()
{
    cursor_erase();
    cursor_x -= CURSOR_STEP_X;
    cursor_draw();
    if (cursor_x < 0)
        display_output_previousline();
}

void display_output_nextline()
{
    cursor_erase();
    cursor_x = 0;
    cursor_y += CURSOR_STEP_Y;
    cursor_draw();
}

void display_output_nextpage()
{
    display_output_init();
}

void display_output_init()
{
    cursor_x = 0;
    cursor_y = 0;
    display_clear();
    cursor_draw();
    display_refresh();
}

void display_output_print(char c)
{
    switch (c)
    {
    case '\b':
        display_output_backspace();
        break;
    case '\t':
        cursor_draw_char(' ');
        cursor_draw_char(' ');
        break;
    case '\r':
    case '\n':
        display_output_nextline();
        break;
    case '\f':
        display_output_nextpage();
        break;
    default:
        if (c >= ' ' && c <= '~')
            cursor_draw_char(c);
        break;
    }
}
