#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "computer.h"
#include "data.h"
#include "game.h"
#include "output.h"

#define INF 1000000
#define MAX_DEPTH 6

static int32_t evaluate_window(int32_t count_ai, int32_t count_opp)
{
    if (count_ai == 4)
        return 10000;
    else if (count_ai == 3 && count_opp == 0)
        return 100;
    else if (count_ai == 2 && count_opp == 0)
        return 10;
    else if (count_opp == 3 && count_ai == 0)
        return -80;
    else if (count_opp == 2 && count_ai == 0)
        return -5;
    return 0;
}

static int32_t evaluate_board(const Bitmap *player_board, const Bitmap *opp_board)
{
    const Point dirs[4] = {{+1, +0}, {+0, +1}, {+1, +1}, {+1, -1}};

    int32_t score = 0;

    int32_t center = WIDTH / 2;
    int32_t center_count = 0;
    for (int32_t r = 0; r < HIGHTH; r++)
    {
        if (player_board->data[center][r])
            center_count++;
    }
    score += center_count * 3;

    for (int32_t d = 0; d < 4; d++)
    {
        for (int32_t r = 0; r < HIGHTH; r++)
        {
            for (int32_t c = 0; c < WIDTH; c++)
            {
                if (c + (CONNECTION - 1) * dirs[d].x < WIDTH && c + (CONNECTION - 1) * dirs[d].x >= 0 &&
                    r + (CONNECTION - 1) * dirs[d].y < HIGHTH && r + (CONNECTION - 1) * dirs[d].y >= 0)
                {
                    int count_ai = 0, count_opp = 0;
                    for (int i = 0; i < 4; i++)
                    {
                        int nc = c + i * dirs[d].x;
                        int nr = r + i * dirs[d].y;

                        if (player_board->data[nc][nr])
                            count_ai++;
                        else if (opp_board->data[nc][nr])
                            count_opp++;
                    }
                    score += evaluate_window(count_ai, count_opp);
                }
            }
        }
    }

    return score;
}

static int32_t negamax(Bitmap player_board, Bitmap opp_board, int32_t depth, int32_t alpha, int32_t beta, int32_t x, int32_t y)
{
    if (bitmap_is_connect(&opp_board, x, y))
        return -INF;
    if (depth == 0 || bitmap_is_full(&player_board, &opp_board))
        return evaluate_board(&player_board, &opp_board);

    int32_t value = -INF;

    int32_t order[WIDTH];
    order[0] = WIDTH / 2;
    int32_t idx = 1;
    for (int32_t offset = 1; offset <= WIDTH / 2; offset++)
    {
        if (WIDTH / 2 - offset >= 0)
            order[idx++] = WIDTH / 2 - offset;
        if (WIDTH / 2 + offset < WIDTH)
            order[idx++] = WIDTH / 2 + offset;
    }

    for (int32_t i = 0; i < WIDTH; i++)
    {
        int32_t col = order[i];
        int32_t row = bitmap_insert(&player_board, &opp_board, col);
        if (row != -1)
        {
            Bitmap new_player_board = player_board;
            new_player_board.data[col][row] = true;
            int32_t score = -negamax(opp_board, new_player_board, depth - 1, -beta, -alpha, col, row);
            if (score > value)
                value = score;
            if (value > alpha)
                alpha = value;
            if (alpha >= beta)
                break;
        }
    }
    return value;
}

bool computer_move(Player *player, Player *competitor)
{
    player->y = -1;
    draw_player_piece(player);

    Bitmap board_ai = player->bitmap;
    Bitmap board_opp = competitor->bitmap;

    int32_t best_score = -INF;
    int32_t best_col = -1;

    int32_t order[WIDTH];
    order[0] = WIDTH / 2;
    int32_t idx = 1;
    for (int32_t offset = 1; offset <= WIDTH / 2; offset++)
    {
        if (WIDTH / 2 - offset >= 0)
            order[idx++] = WIDTH / 2 - offset;
        if (WIDTH / 2 + offset < WIDTH)
            order[idx++] = WIDTH / 2 + offset;
    }

    for (int32_t i = 0; i < WIDTH; i++)
    {
        int32_t col = order[i];
        int32_t row = bitmap_insert(&board_ai, &board_opp, col);
        if (row != -1)
        {
            Bitmap new_board_ai = board_ai;
            new_board_ai.data[col][row] = true;
            int32_t score = -negamax(board_opp, new_board_ai, MAX_DEPTH - 1, -INF, INF, col, row);
            if (score > best_score)
            {
                best_score = score;
                best_col = col;
            }
        }
    }

    if (best_col == -1)
        return false;

    draw_player_erase_piece(player);
    player->x = best_col;
    player_insert(player, competitor);
    draw_player_piece(player);

    return true;
}
