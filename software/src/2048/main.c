#include <string.h>

#include "five_way_motion_sensor.h"
#include "display_96_64.h"
#include "random_number_generator.h"
#include "driver.h"

#include "2048_grid.h"

#define BOARD_SIZE 4

#define SCORE_STR_X (DISPLAY_SIZE_X - 30)
#define SCORE_STR_Y 0
#define SCORE_NUM_X DISPLAY_SIZE_X
#define SCORE_NUM_Y 8

signed char board[BOARD_SIZE][BOARD_SIZE];
unsigned int score;
char score_str[8];

void game_init();
void random_tile_generate();
signed char slide(signed char dx, signed char dy);
void display_update();
signed char game_over_check();
void draw_score();
void draw_number(char x, char y, char *str);
char *itoa(int value, char *str, int base);

int main()
{
    while (1)
    {
        game_init();

        while (1)
        {
            signed char moved = 0;

            motion_sensor_refresh();
            if (motion_sensor_l())
                moved = slide(1, 0);
            else if (motion_sensor_r())
                moved = slide(-1, 0);
            else if (motion_sensor_u())
                moved = slide(0, -1);
            else if (motion_sensor_d())
                moved = slide(0, 1);

            if (moved)
            {
                random_tile_generate();
                display_update();
                if (game_over_check())
                {
                    draw_grid_int(DISPLAY_SIZE_X / 2, DISPLAY_SIZE_Y / 2, 32, 9, game_over_grid[0]);
                    draw_grid_int(DISPLAY_SIZE_X / 2 - 32, DISPLAY_SIZE_Y / 2, 32, 9, game_over_grid[1]);
                    display_refresh();
                    break;
                }
                motion_sensor_refresh();
            }
        }

        driver_off();
    }
    return 0;
}

void game_init()
{
    display_clear();
    score = 0;
    memset(board, 0, BOARD_SIZE * BOARD_SIZE);
    random_tile_generate();
    random_tile_generate();
    display_update();
    draw_score();
    motion_sensor_refresh();
    display_refresh();
}

void random_tile_generate()
{
    unsigned char empty_tiles[BOARD_SIZE * BOARD_SIZE][2];
    unsigned char empty_count = 0;

    for (unsigned char i = 0; i < BOARD_SIZE; i++)
    {
        for (unsigned char j = 0; j < BOARD_SIZE; j++)
        {
            if (board[i][j] == 0)
            {
                empty_tiles[empty_count][0] = i;
                empty_tiles[empty_count][1] = j;
                empty_count++;
            }
        }
    }

    if (empty_count > 0)
    {
        unsigned char rand_index = rand.u32 % empty_count;
        unsigned char x = empty_tiles[rand_index][0];
        unsigned char y = empty_tiles[rand_index][1];
        board[x][y] = rand.u32 % 10 == 0 ? 2 : 1;
    }
}

signed char slide(signed char dx, signed char dy)
{
    signed char moved = 0;

    for (unsigned char i = 0; i < BOARD_SIZE; i++)
    {
        signed char *line[BOARD_SIZE];
        signed char fill_position = 0;

        for (unsigned char j = 0; j < BOARD_SIZE; j++)
        {
            signed char x = dx == 0 ? i : (dx > 0 ? BOARD_SIZE - 1 - j : j);
            signed char y = dy == 0 ? i : (dy > 0 ? BOARD_SIZE - 1 - j : j);

            line[j] = &board[x][y];
        }

        for (unsigned char j = 0; j < BOARD_SIZE; j++)
        {
            if (*line[j] != 0)
            {
                if (j != fill_position)
                {
                    *line[fill_position] = *line[j];
                    *line[j] = 0;
                    moved = 1;
                }
                fill_position++;
            }
        }

        for (unsigned char j = 0; j < fill_position - 1; j++)
        {
            if (*line[j] == *line[j + 1])
            {
                *line[j] += 1;
                *line[j + 1] = 0;
                score += 1 << *line[j];
                moved = 1;
            }
        }

        fill_position = 0;
        for (unsigned char j = 0; j < BOARD_SIZE; j++)
        {
            if (*line[j] != 0)
            {
                if (j != fill_position)
                {
                    *line[fill_position] = *line[j];
                    *line[j] = 0;
                }
                fill_position++;
            }
        }
    }
    return moved;
}

void display_update()
{
    display_clear();
    draw_score();
    for (unsigned char i = 0; i < BOARD_SIZE; i++)
        for (unsigned char j = 0; j < BOARD_SIZE; j++)
            if (board[i][j] != 0)
                draw_grid_short((GRID_SIZE + 1) * i,
                                (GRID_SIZE + 1) * j,
                                GRID_SIZE,
                                GRID_SIZE,
                                board_grid[board[i][j] - 1]);
    display_refresh();
}

signed char game_over_check()
{
    for (unsigned char i = 0; i < BOARD_SIZE; i++)
    {
        for (unsigned char j = 0; j < BOARD_SIZE; j++)
        {
            if (board[i][j] == 0)
                return 0;
            if (i > 0 && board[i][j] == board[i - 1][j])
                return 0;
            if (i < BOARD_SIZE - 1 && board[i][j] == board[i + 1][j])
                return 0;
            if (j > 0 && board[i][j] == board[i][j - 1])
                return 0;
            if (j < BOARD_SIZE - 1 && board[i][j] == board[i][j + 1])
                return 0;
        }
    }
    return 1;
}

void draw_score()
{
    draw_grid_int(SCORE_STR_X, SCORE_STR_Y, 30, 7, score_grid);
    itoa(score, score_str, 10);
    draw_number(SCORE_NUM_X, SCORE_NUM_Y, score_str);
}

void draw_number(char x, char y, char *str)
{
    unsigned char i = 0;
    while (score_str[i] != '\0')
    {
        x -= 4;
        draw_grid_char(x, y, 3, 5, number_grid[str[i++] - '0']);
    }
}

char *itoa(int value, char *str, int base)
{
    char *ret = str;
    char negative = 0;
    if (value == 0)
    {
        *str++ = '0';
    }
    else
    {
        int sum;

        if (base == 10 && value < 0)
        {
            value = -value;
            negative = 1;
        }

        while (value != 0)
        {
            sum = value % base;
            *str++ = (sum > 9) ? (char)(sum - 10) + 'a' : (char)sum + '0';
            value /= base;
        }

        if (negative)
            *str++ = '-';

        for (char *left = ret, *right = str - 1; left < right; left++, right--)
        {
            char tmp = *left;
            *left = *right;
            *right = tmp;
        }
    }
    *str = '\0';
    return ret;
}
