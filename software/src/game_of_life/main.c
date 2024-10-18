#include <string.h>

#include "display_96_64.h"
#include "board.h"

void update_neighbors(unsigned char board[][GAME_WIDTH], int x, int y, int delta)
{
    board[y - 1][x - 1] += delta;
    board[y - 1][x] += delta;
    board[y - 1][x + 1] += delta;
    board[y][x - 1] += delta;
    board[y][x + 1] += delta;
    board[y + 1][x - 1] += delta;
    board[y + 1][x] += delta;
    board[y + 1][x + 1] += delta;
}

void update_board(unsigned char board_old[][GAME_WIDTH],
                  unsigned char board_new[][GAME_WIDTH])
{
    unsigned char neighbors_alive;

    memcpy(board_new, board_old, GAME_HEIGHT * GAME_WIDTH);

    for (int x = 1; x < GAME_WIDTH - 1; x++)
        for (int y = 1; y < GAME_HEIGHT - 1; y++)
        {
            neighbors_alive = (board_old[y][x] & 0b1110) >> 1;
            if ((board_old[y][x] & 1) == 0)
            {
                if (neighbors_alive == 3)
                {
                    board_new[y][x] |= 1;
                    update_neighbors(board_new, x, y, 0b10);
                    draw_pixel(x, y, 1);
                }
            }
            else
            {
                if (neighbors_alive != 2 && neighbors_alive != 3)
                {
                    board_new[y][x] &= ~1;
                    update_neighbors(board_new, x, y, -0b10);
                    draw_pixel(x, y, 0);
                }
            }
        }
}

void init_board(unsigned char board[][GAME_WIDTH])
{
    for (int y = 0; y < GAME_HEIGHT; y++)
        for (int x = 0; x < GAME_WIDTH; x++)
        {
            draw_pixel(x, y, board[y][x] & 1);

            if (x == 0 || x == GAME_WIDTH - 1 ||
                y == 0 || y == GAME_HEIGHT - 1)
            {
                board[y][x] = 0b1110;
            }
            else
            {
                board[y][x] |= ((board[y - 1][x - 1] & 1) +
                                (board[y - 1][x] & 1) +
                                (board[y - 1][x + 1] & 1) +
                                (board[y][x - 1] & 1) +
                                (board[y][x + 1] & 1) +
                                (board[y + 1][x - 1] & 1) +
                                (board[y + 1][x] & 1) +
                                (board[y + 1][x + 1] & 1))
                               << 1;
            }
        }
}

int main()
{
    unsigned char step = 0;
    init_board(board_buffer[0]);
    display_refresh();
    while (1)
    {
        update_board(board_buffer[step & 1], board_buffer[(step + 1) & 1]);
        display_refresh();
        step++;
    }
    return 0;
}