#include <stdbool.h>
#include <stdint.h>

#include "display_96_64.h"

#include "data.h"

#define TEXT_GRID_W 31
#define TEXT_GRID_H 5
#define TEXT_POS_X 3
#define TEXT_POS_Y 15

#define PIECE_GRID_W 7
#define PIECE_GRID_H 7
#define PIECE_POS_X 39
#define PIECE_POS_Y 15

const uint32_t player_grid[TEXT_GRID_H] = {
    0b1100100001001010111011000000000,
    0b1010100010101010100010100000000,
    0b1100100011100100110011000000000,
    0b1000100010100100100010100000000,
    0b1000111010100100111010100000000,
};
const uint32_t computer_grid[TEXT_GRID_H] = {
    0b0110010010101100101011101110110,
    0b1000101011101010101001001000101,
    0b1000101011101100101001001100110,
    0b1000101011101000101001001000101,
    0b0110010011101000111001001110101,
};
const uint32_t nobody_grid[TEXT_GRID_H] = {
    0b1010010011001100101000000000000,
    0b1110101010101010101000000000000,
    0b1110101011001010010000000000000,
    0b1110101010101010010000000000000,
    0b1010010011001100010000000000000,
};
const uint32_t win_grid[TEXT_GRID_H] = {
    0b1000100111001000100000000000000,
    0b1010100010001100100000000000000,
    0b1010100010001010100000000000000,
    0b1010100010001001100000000000000,
    0b0101000111001000100000000000000,

};

const uint8_t black_piece_grid[PIECE_GRID_H] = {
    0b0011100,
    0b0100010,
    0b1000001,
    0b1000001,
    0b1000001,
    0b0100010,
    0b0011100,
};
const uint8_t white_piece_grid[PIECE_GRID_H] = {
    0b0011100,
    0b0111110,
    0b1111111,
    0b1111111,
    0b1111111,
    0b0111110,
    0b0011100,
};
const uint8_t erase_piece_grid[PIECE_GRID_H] = {0};

void draw_pixel_switch(Point point)
{
    if ((_display_buffer[point.x / 8 + point.y * 16] >> (point.x % 8)) & 1)
        draw_pixel(point.x, point.y, 0);
    else
        draw_pixel(point.x, point.y, 1);
}

void draw_win_piece(Point *pos)
{
    for (int32_t i = 0; i < CONNECTION; i++)
    {
        Point point = {
            PIECE_POS_X + PIECE_GRID_W / 2 + (PIECE_GRID_W + 1) * pos[i].x,
            PIECE_POS_Y + PIECE_GRID_H / 2 + (PIECE_GRID_H + 1) * pos[i].y,
        };
        draw_pixel_switch(point);
    }
    display_refresh();
}

void draw_nobody_text()
{
    draw_grid_int(TEXT_POS_X, TEXT_POS_Y, TEXT_GRID_W, TEXT_GRID_H, (const unsigned int *)nobody_grid);
    display_refresh();
}

void draw_player_text()
{
    draw_grid_int(TEXT_POS_X, TEXT_POS_Y, TEXT_GRID_W, TEXT_GRID_H, (const unsigned int *)player_grid);
    display_refresh();
}

void draw_computer_text()
{
    draw_grid_int(TEXT_POS_X, TEXT_POS_Y, TEXT_GRID_W, TEXT_GRID_H, (const unsigned int *)computer_grid);
    display_refresh();
}

void draw_win_text()
{
    draw_grid_int(TEXT_POS_X, TEXT_POS_Y + TEXT_GRID_H + 1, TEXT_GRID_W, TEXT_GRID_H, (const unsigned int *)win_grid);
    display_refresh();
}

void draw_piece(int32_t x, int32_t y, const uint8_t *grid)
{
    draw_grid_char(PIECE_POS_X + (PIECE_GRID_W + 1) * x,
                   PIECE_POS_Y + (PIECE_GRID_H + 1) * y,
                   PIECE_GRID_W, PIECE_GRID_H, grid);
}

void draw_player_erase_piece(Player *player)
{
    draw_piece(player->x, player->y, erase_piece_grid);
}

void draw_player_piece(Player *player)
{
    draw_piece(player->x, player->y,
               player->color ? white_piece_grid : black_piece_grid);
    display_refresh();
}

void draw_init()
{
    display_clear();
    draw_line(PIECE_POS_X - 2,
              PIECE_POS_Y,
              PIECE_POS_X - 2,
              PIECE_POS_Y + (PIECE_GRID_H + 1) * HIGHTH,
              solid_line_one_func);
    draw_line(PIECE_POS_X + (PIECE_GRID_W + 1) * WIDTH,
              PIECE_POS_Y,
              PIECE_POS_X + (PIECE_GRID_W + 1) * WIDTH,
              PIECE_POS_Y + (PIECE_GRID_H + 1) * HIGHTH,
              solid_line_one_func);
    draw_line(PIECE_POS_X - 2,
              PIECE_POS_Y + (PIECE_GRID_H + 1) * HIGHTH,
              PIECE_POS_X + (PIECE_GRID_W + 1) * WIDTH,
              PIECE_POS_Y + (PIECE_GRID_H + 1) * HIGHTH,
              solid_line_one_func);
    display_refresh();
}
