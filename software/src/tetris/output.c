#include "display_96_64.h"
#include "driver.h"

#include "output.h"
#include "tetromino.h"
#include "matrix.h"
#include "preview.h"
#include "hold.h"
#include "data.h"
#include "helper.h"

#define TEXT_GRID_HIGH 5
#define TEXT_GRID_WIDTH 29

#define NUM_GRID_HIGH 5
#define NUM_GRID_WIDTH 3

#define NUM_MAX_BIT 11

#define SCORE_POS_X 72
#define SCORE_POS_Y 28
#define LEVEL_POS_X 72
#define LEVEL_POS_Y 42
#define LINES_POS_X 72
#define LINES_POS_Y 56

const unsigned int score_text_grid[TEXT_GRID_HIGH] = {
    0b01111001110001110011110011111,
    0b10000010001010001010001010000,
    0b01110010000010001011110011110,
    0b00001010001010001010010010000,
    0b11110001110001110010001011111,
};

const unsigned int level_text_grid[TEXT_GRID_HIGH] = {
    0b10000011111010001011111010000,
    0b10000010000010001010000010000,
    0b10000011110010001011110010000,
    0b10000010000001010010000010000,
    0b11111011111000100011111011111,
};

const unsigned int lines_text_grid[TEXT_GRID_HIGH] = {
    0b10000001110010001011111001111,
    0b10000000100011001010000010000,
    0b10000000100010101011110001110,
    0b10000000100010011010000000001,
    0b11111001110010001011111011110,
};

const unsigned char number_grid[10][NUM_GRID_HIGH] = {
    {0b111, 0b101, 0b101, 0b101, 0b111},
    {0b010, 0b110, 0b010, 0b010, 0b111},
    {0b110, 0b001, 0b010, 0b100, 0b111},
    {0b110, 0b001, 0b010, 0b001, 0b110},
    {0b101, 0b101, 0b111, 0b001, 0b001},
    {0b111, 0b100, 0b110, 0b001, 0b110},
    {0b011, 0b100, 0b111, 0b101, 0b111},
    {0b111, 0b001, 0b010, 0b010, 0b010},
    {0b111, 0b101, 0b111, 0b101, 0b111},
    {0b111, 0b101, 0b111, 0b001, 0b110},
};

typedef union
{
    unsigned short s;
    unsigned char c[2];
} Union16Data;

typedef union
{
    unsigned int i;
    unsigned short s[2];
} Union32Data;

unsigned char *get_8bit_addr(signed char x, signed char y)
{
    return &_display_buffer[x / 8 + y * 16];
}

unsigned short *get_16bit_addr(signed char x, signed char y)
{
    return (unsigned short *)&_display_buffer[(x / 16) * 2 + y * 16];
}

Union16Data get_16bit_mask(signed char x, signed char w)
{
    Union16Data mask;

    signed char low = x % 8;
    signed char high = low + w;

    mask.s = (1 << high) - (1 << low);

    return mask;
}

Union32Data get_32bit_mask(signed char x, signed char w)
{
    Union32Data mask;

    signed char low = x % 16;
    signed char high = low + w;

    mask.i = (1 << high) - (1 << low);

    return mask;
}

void draw_grid_copy_up_down(signed char x, signed char from_y, signed char to_y, signed char w, signed char h)
{
    unsigned short *from_addr = get_16bit_addr(x, from_y > to_y ? from_y : from_y + h - 1);
    unsigned short *to_addr = get_16bit_addr(x, from_y > to_y ? to_y : to_y + h - 1);

    signed char da = from_y > to_y ? 8 : -8;

    Union32Data mask = get_32bit_mask(x, w);

    for (unsigned char i = 0; i < h; i++, from_addr += da, to_addr += da)
    {
        *(to_addr + 0) = (*(to_addr + 0) & ~mask.s[0]) ^ (*(from_addr + 0) & mask.s[0]);
        *(to_addr + 1) = (*(to_addr + 1) & ~mask.s[1]) ^ (*(from_addr + 1) & mask.s[1]);
    }
}

void draw_tetromino_grid_matrix(signed char mx, signed char my, unsigned short *grid, signed char pixel)
{
    signed char x = MATRIX_POS_X + TETROMINO_GRID_W * (mx - 0);
    signed char y = MATRIX_POS_Y + TETROMINO_GRID_H * (my - MATRIX_VISUAL_UPPER);
    signed char low = x % 16;

    unsigned short *addr = get_16bit_addr(x, y);

    Union32Data data;

    for (signed char i = 0; i < TETROMINO_H * TETROMINO_GRID_H; i++, addr += 8)
    {
        if (y + i < MATRIX_POS_Y || y + i >= MATRIX_POS_Y + (MATRIX_H * TETROMINO_GRID_H) || grid[i] == 0)
            continue;

        data.i = (unsigned int)(grid[i] << low);

        if (pixel)
        {
            *(addr + 0) |= data.s[0];
            *(addr + 1) |= data.s[1];
        }
        else
        {
            *(addr + 0) &= ~data.s[0];
            *(addr + 1) &= ~data.s[1];
        }
    }
}

void draw_matrix(signed char x, signed char y, signed char w, signed char h, unsigned int *board)
{
    unsigned int grid[h * TETROMINO_GRID_H];
    unsigned int value, mask = (1 << TETROMINO_GRID_W) - 1;

    for (unsigned char j = y, m = 0; j < y + h; j++, m++)
    {
        value = 0;

        for (unsigned char i = x, n = 0; i < x + w; i++, n++)
            if ((board[i] >> j) & 1)
                value |= mask << (n * TETROMINO_GRID_W);

        for (unsigned char i = 0; i < TETROMINO_GRID_H; i++)
            grid[m * TETROMINO_GRID_H + i] = value;
    }

    draw_grid_int(MATRIX_POS_X + (x - 0) * TETROMINO_GRID_W,
                  MATRIX_POS_Y + (y - MATRIX_VISUAL_UPPER) * TETROMINO_GRID_H,
                  w * TETROMINO_GRID_W,
                  h * TETROMINO_GRID_H,
                  grid);
}

void draw_tetromino_grid_up(signed char x, signed char y, TetroType type)
{
    unsigned char board[TETROMINO_W];
    unsigned short grid[TETROMINO_H * TETROMINO_GRID_H];

    signed char dx, dy, dw, dh;
    const signed char d = ((TETROMINO_H - TETROMINO_UP_H) / 2 * TETROMINO_GRID_H);

    tetromino_board_set(board, type, DIR_U);
    tetromino_grid_set(grid, board);
    tetromino_size_get(type, DIR_U, &dw, &dh);

    dx = (TETROMINO_UP_W - dw) * TETROMINO_GRID_W / 2;
    dy = (TETROMINO_UP_H - dh) * TETROMINO_GRID_H / 2;

    for (signed char i = TETROMINO_H * TETROMINO_GRID_H - d - dy - 1; i >= d - 1; i--)
        grid[i + dy] = grid[i] >> dx;

    draw_grid_short(x, y,
                    TETROMINO_UP_W * TETROMINO_GRID_W,
                    TETROMINO_UP_H * TETROMINO_GRID_H,
                    grid + d);
}

void draw_score_ui()
{
    draw_grid_int(SCORE_POS_X - TEXT_GRID_WIDTH / 2,
                  SCORE_POS_Y - TEXT_GRID_HIGH - 2,
                  TEXT_GRID_WIDTH,
                  TEXT_GRID_HIGH,
                  score_text_grid);
}

void draw_level_ui()
{
    draw_grid_int(LEVEL_POS_X - TEXT_GRID_WIDTH / 2,
                  LEVEL_POS_Y - TEXT_GRID_HIGH - 2,
                  TEXT_GRID_WIDTH,
                  TEXT_GRID_HIGH,
                  level_text_grid);
}

void draw_lines_ui()
{
    draw_grid_int(LINES_POS_X - TEXT_GRID_WIDTH / 2,
                  LINES_POS_Y - TEXT_GRID_HIGH - 2,
                  TEXT_GRID_WIDTH,
                  TEXT_GRID_HIGH,
                  lines_text_grid);
}

void draw_numbers(signed char x, signed char y, unsigned int value)
{
    char str[NUM_MAX_BIT + 1];

    signed char length = itoa_l(value, str, 10);

    x += (length + 1) * (NUM_GRID_WIDTH + 1) / 2 - 1;

    unsigned char i = 0;
    while (str[i] != '\0')
    {
        x -= NUM_GRID_WIDTH + 1;
        draw_grid_char(x, y, NUM_GRID_WIDTH + 1, NUM_GRID_HIGH, number_grid[str[i++] - '0']);
    }
}

void draw_score()
{
    draw_numbers(SCORE_POS_X, SCORE_POS_Y, score);
}

void draw_level()
{
    draw_numbers(LEVEL_POS_X, LEVEL_POS_Y, level);
}

void draw_lines()
{
    draw_numbers(LINES_POS_X, LINES_POS_Y, lines);
}

void draw_ui_rect(int x, int y, int w, int h)
{
    draw_line(x, y, x + w - 1, y, solid_line_one_func);
    draw_line(x, y + h - 1, x + w - 1, y + h - 1, solid_line_one_func);
    draw_line(x, y, x, y + h - 1, solid_line_one_func);
    draw_line(x + w - 1, y, x + w - 1, y + h - 1, solid_line_one_func);
}

void draw_matrix_ui()
{
    draw_ui_rect(MATRIX_POS_X - 2,
                 MATRIX_POS_Y - 2,
                 MATRIX_W * TETROMINO_GRID_W + 4,
                 MATRIX_H * TETROMINO_GRID_H + 4);
}

void draw_preview_ui()
{
    draw_ui_rect(PREVIEW_POS_X - 2,
                 PREVIEW_POS_Y - 2,
                 TETROMINO_UP_W * TETROMINO_GRID_W + 4,
                 (TETROMINO_UP_H * TETROMINO_GRID_H + 2) * PREVIEW_LENGTH + 3);
}

void draw_hold_ui()
{
    draw_ui_rect(HOLD_POS_X - 2,
                 HOLD_POS_Y - 2,
                 TETROMINO_UP_W * TETROMINO_GRID_W + 4,
                 TETROMINO_UP_H * TETROMINO_GRID_H + 4);
}

void draw_ui()
{
    display_clear();

    draw_matrix_ui();
    draw_preview_ui();
    draw_hold_ui();

    draw_level_ui();
    draw_lines_ui();
    draw_score_ui();

    draw_refresh();
}

void draw_refresh()
{
    display_refresh();
}

void draw_clear()
{
    display_clear();
}

void interrupt()
{
    driver_off();
}
