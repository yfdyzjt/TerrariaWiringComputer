#include "tetromino.h"

const Point srs_table[SRS_TABLE_NUMBER][DIR_NUMBER][SRS_TEST_NUMBER] = {
    {
        {{+0, +0}, {+0, +0}, {+0, +0}, {+0, +0}, {+0, +0}},
        {{+0, +0}, {-1, +0}, {-1, +1}, {+0, -2}, {-1, -2}},
        {{+0, +0}, {+0, +0}, {+0, +0}, {+0, +0}, {+0, +0}},
        {{+0, +0}, {+1, +0}, {+1, +1}, {+0, -2}, {+1, -2}},
    },
    {
        {{+0, +0}, {+1, +0}, {-2, +0}, {+1, +0}, {-2, +0}},
        {{+0, +0}, {-1, +0}, {-1, +0}, {-1, -1}, {-1, +2}},
        {{+0, +0}, {-2, +0}, {+1, +0}, {-2, +1}, {+1, +1}},
        {{+1, +0}, {+1, +0}, {+1, +0}, {+1, +2}, {+1, -1}},
    },
    {
        {{+0, +0}, {+0, +0}, {+0, +0}, {+0, +0}, {+0, +0}},
        {{+0, +0}, {+0, +0}, {+0, +0}, {+0, +0}, {+0, +0}},
        {{+0, +1}, {+0, +1}, {+0, +1}, {+0, +1}, {+0, +1}},
        {{+1, +0}, {+1, +0}, {+1, +0}, {+1, +0}, {+1, +0}},
    },
};

const unsigned char tetro_table[TETRO_NUMBER][DIR_NUMBER] = {
    {0b00111010, 0b10001110, 0b00010111, 0b11100010},
    {0b00101011, 0b00101110, 0b00110101, 0b11101000},
    {0b00101110, 0b01001110, 0b00011101, 0b11100100},
    {0b00011110, 0b11000110, 0b00011110, 0b11000110},
    {0b00101101, 0b01101100, 0b00101101, 0b01101100},
    {0b01010101, 0b11110000, 0b01010101, 0b00001111},
    {0b00111100, 0b01100110, 0b00111100, 0b01100110},
};

void tetromino_board_set(unsigned char *board, TetroType type, DirType dir)
{
    switch (dir)
    {
    case DIR_U:
        board[0] = (tetro_table[type][dir] >> 5) & 0b0110;
        board[1] = (tetro_table[type][dir] >> 3) & 0b0110;
        board[2] = (tetro_table[type][dir] >> 1) & 0b0110;
        board[3] = (tetro_table[type][dir] << 1) & 0b0110;
        break;
    case DIR_R:
        board[0] = 0;
        board[1] = (tetro_table[type][dir] >> 4) & 0b1111;
        board[2] = (tetro_table[type][dir] >> 0) & 0b1111;
        board[3] = 0;
        break;
    case DIR_D:
        board[0] = (tetro_table[type][dir] >> 4) & 0b1100;
        board[1] = (tetro_table[type][dir] >> 2) & 0b1100;
        board[2] = (tetro_table[type][dir] >> 0) & 0b1100;
        board[3] = (tetro_table[type][dir] << 2) & 0b1100;
        break;
    case DIR_L:
        board[0] = 0;
        board[1] = 0;
        board[2] = (tetro_table[type][dir] >> 4) & 0b1111;
        board[3] = (tetro_table[type][dir] >> 0) & 0b1111;
        break;
    default:
        break;
    }
}

void tetromino_grid_set(unsigned short *grid, unsigned char *board)
{
    unsigned short value, mask = (1 << TETROMINO_GRID_W) - 1;

    for (unsigned char j = 0; j < TETROMINO_H; j++)
    {
        value = 0;

        for (unsigned char i = 0; i < TETROMINO_W; i++)
            if ((board[i] >> j) & 1)
                value |= mask << (i * TETROMINO_GRID_W);

        for (unsigned char k = 0; k < TETROMINO_GRID_H; k++)
            grid[j * TETROMINO_GRID_H + k] = value;
    }
}

void tetromino_ghost_grid_set(unsigned short *grid, unsigned char *board)
{
    unsigned short value, mask = 1 << (TETROMINO_GRID_W / 2);

    for (unsigned char j = 0; j < TETROMINO_H; j++)
    {
        value = 0;

        for (unsigned char i = 0; i < TETROMINO_W; i++)
            if ((board[i] >> j) & 1)
                value |= mask << (i * TETROMINO_GRID_W);

        grid[j * TETROMINO_GRID_H + (TETROMINO_GRID_W / 2)] = value;
    }
}

void tetromino_size_get(TetroType type, DirType dt, signed char *w, signed char *h)
{
    signed char wt, ht;

    switch (type)
    {
    case TETRO_J:
    case TETRO_L:
    case TETRO_T:
    case TETRO_S:
    case TETRO_Z:
        wt = 3;
        ht = 2;
        break;
    case TETRO_I:
        wt = 4;
        ht = 1;
        break;
    case TETRO_O:
        wt = 4;
        ht = 2;
        break;
    default:
        wt = 0;
        ht = 0;
        break;
    }

    *w = dt & 1 ? ht : wt;
    *h = dt & 1 ? wt : ht;
}

void tetromino_srs_move_get(TetroType type, DirType ds, DirType dt, unsigned char test_num, signed char *dx, signed char *dy)
{
    signed char st;

    switch (type)
    {
    case TETRO_J:
    case TETRO_L:
    case TETRO_T:
    case TETRO_S:
    case TETRO_Z:
        st = 0;
        break;
    case TETRO_I:
        st = 1;
        break;
    case TETRO_O:
        st = 2;
        break;
    default:
        st = 0;
        break;
    }

    *dx = srs_table[st][ds][test_num].x - srs_table[st][dt][test_num].x;
    *dy = srs_table[st][ds][test_num].y - srs_table[st][dt][test_num].y;
}
