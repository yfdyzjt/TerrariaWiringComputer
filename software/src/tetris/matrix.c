#include "matrix.h"
#include "tetromino.h"
#include "output.h"
#include "input.h"
#include "preview.h"
#include "hold.h"
#include "helper.h"
#include "data.h"
#include "score.h"

unsigned int matrix_board[MATRIX_W];

Tetromino tetro, tetro_ghost;

void matrix_tetromino_ghost_pos_set()
{
    unsigned char tetro_zero, tetro_one;
    unsigned char dy, min_dy = MATRIX_VISUAL_LOWER;

    for (unsigned char i = 0; i < TETROMINO_W; i++)
        if (tetro.board[i] != 0)
        {
            tetro_zero = zero_count(tetro.board[i]);
            tetro_one = zero_count(~(tetro.board[i] >> tetro_zero));
            dy = zero_count(matrix_board[tetro.x + i] >> (tetro.y + tetro_zero + tetro_one));
            min_dy = dy < min_dy ? dy : min_dy;
        }

    tetro_ghost.x = tetro.x;
    tetro_ghost.y = tetro.y + min_dy;
}

void matrix_tetromino_draw(signed char pixel)
{
    draw_tetromino_grid_matrix(tetro.x, tetro.y, tetro.grid, pixel);
    draw_tetromino_grid_matrix(tetro_ghost.x, tetro_ghost.y, tetro_ghost.grid, pixel);
}

void matrix_tetromino_grid_set()
{
    tetromino_grid_set(tetro.grid, tetro.board);
    tetromino_ghost_grid_set(tetro_ghost.grid, tetro.board);
}

signed char matrix_perfect_clear_test()
{
    for (unsigned char i = 0; i < TETROMINO_W; i++)
        if (matrix_board[i] != 0x80000000)
            return 0;
    return 1;
}

signed char matrix_pixel_test(signed char x, signed char y)
{
    if (x < 0 || x >= MATRIX_W || ((matrix_board[x] >> y) & 1) != 0)
        return 1;
    else
        return 0;
}

signed char matrix_tetromino_test(signed char x, signed char y)
{
    for (unsigned char i = 0; i < TETROMINO_W; i++)
        if (((x + i < 0 || x + i >= MATRIX_W) && tetro.board[i] != 0) ||
            (matrix_board[x + i] & ((unsigned int)tetro.board[i] << y)) != 0)
            return 1;
    return 0;
}

signed char matrix_tetromino_t_spin_test()
{
    signed char number = 0;
    number += matrix_pixel_test(tetro.x + 1, tetro.y + 1);
    number += matrix_pixel_test(tetro.x + 1, tetro.y + 3);
    number += matrix_pixel_test(tetro.x + 3, tetro.y + 1);
    number += matrix_pixel_test(tetro.x + 3, tetro.y + 3);
    return number >= 3;
}

signed char matrix_tetromino_move_test(signed char dx, signed char dy)
{
    if (!matrix_tetromino_test(tetro.x + dx, tetro.y + dy))
    {
        tetro.x += dx;
        tetro.y += dy;

        matrix_tetromino_ghost_pos_set();

        return 1;
    }
    else
        return 0;
}

signed char matrix_tetromino_move(signed char dx, signed char dy)
{
    matrix_tetromino_draw(0);

    signed char value = matrix_tetromino_move_test(dx, dy);

    matrix_tetromino_draw(1);

    is_t_spin = 0;
    is_mini_t_spin = 0;

    return value;
}

signed char matrix_tetromino_spin(signed char dr)
{
    unsigned char n;
    signed char dx, dy;
    DirType dt = (DirType)((((tetro.dir + dr) % DIR_NUMBER) + DIR_NUMBER) % DIR_NUMBER);

    tetromino_board_set(tetro.board, tetro.type, dt);
    matrix_tetromino_draw(0);

    for (n = 0; n < SRS_TEST_NUMBER; n++)
    {
        tetromino_srs_move_get(tetro.type, tetro.dir, dt, n, &dx, &dy);

        if (matrix_tetromino_move_test(dx, dy))
            goto rotate_success;
    }

    tetromino_board_set(tetro.board, tetro.type, tetro.dir);
    matrix_tetromino_draw(1);

    if (tetro.type == TETRO_T)
    {
        if (matrix_tetromino_t_spin_test())
        {
            if (n == 0)
                is_t_spin = 1;
            else
                is_mini_t_spin = 1;
        }
    }

    return 0;

rotate_success:
    tetro.dir = dt;

    matrix_tetromino_grid_set();
    matrix_tetromino_draw(1);

    return 1;
}

void matrix_tetromino_clear_lines()
{
    unsigned int left, right;
    unsigned char result = 0, number = 0;

    for (unsigned char i = 0; i < TETROMINO_W; i++)
        result |= tetro.board[i];

    for (unsigned char i = 0; i < MATRIX_W; i++)
        result &= (unsigned char)(matrix_board[i] >> tetro.y);

    for (unsigned char i = 0; i < TETROMINO_H; i++)
        if ((result >> i) & 1)
        {
            number++;
            right = (1 << (tetro.y + i)) - 1;
            left = ~((1 << (tetro.y + i + 1)) - 1);

            for (unsigned char j = 0; j < MATRIX_W; j++)
                matrix_board[j] = (matrix_board[j] & left) | ((matrix_board[j] & right) << 1);

            if (i > 0)
                draw_grid_copy_up_down(MATRIX_POS_X,
                                       MATRIX_POS_Y + (tetro.y - MATRIX_VISUAL_UPPER) * TETROMINO_GRID_H,
                                       MATRIX_POS_Y + (tetro.y - MATRIX_VISUAL_UPPER + 1) * TETROMINO_GRID_H,
                                       MATRIX_W * TETROMINO_GRID_W,
                                       i * TETROMINO_GRID_H);
        }

    if (number > 0)
    {
        draw_grid_copy_up_down(MATRIX_POS_X,
                               MATRIX_POS_Y,
                               MATRIX_POS_Y + number * TETROMINO_GRID_H,
                               MATRIX_W * TETROMINO_GRID_W,
                               (tetro.y - MATRIX_VISUAL_UPPER) * TETROMINO_GRID_H);

        draw_matrix(0, MATRIX_VISUAL_UPPER, MATRIX_W, number, matrix_board);

        lines_add(number);
    }

    if (matrix_perfect_clear_test())
        is_perfect_clear = 1;

    is_hold = 0;

    score_lock(number);
}

signed char matrix_tetromino_spawn(TetroType type)
{
    tetro.type = type;
    tetro.dir = DIR_U;

    tetro.x = (MATRIX_W - TETROMINO_UP_W) / 2;
    tetro.y = MATRIX_VISUAL_UPPER - (TETROMINO_H - TETROMINO_UP_H) / 2;

    tetromino_board_set(tetro.board, tetro.type, tetro.dir);

    for (unsigned char i = 0; i < SPAWN_TEST_NUMBER; i++)
        if (matrix_tetromino_move_test(0, -i))
        {
            matrix_tetromino_grid_set();
            matrix_tetromino_draw(1);
            return 1;
        }

    is_game_over = 1;
    return 0;
}

signed char matrix_tetromino_hold()
{
    matrix_tetromino_draw(0);

    TetroType new_type = hold_get_set(tetro.type);

    if (new_type == TETRO_NIL)
        return matrix_tetromino_spawn(preview_get());
    else
        return matrix_tetromino_spawn(new_type);
}

signed char matrix_tetromino_can_move_number()
{
    return tetro_ghost.y - tetro.y;
}

signed char matrix_tetromino_lock()
{
    for (unsigned char i = 0; i < TETROMINO_W; i++)
        if (tetro.board[i] != 0)
            matrix_board[tetro.x + i] |= tetro.board[i] << tetro.y;

    matrix_tetromino_clear_lines();
    return matrix_tetromino_spawn(preview_get());
}

void matrix_init()
{
    for (unsigned char i = 0; i < MATRIX_W; i++)
        matrix_board[i] = 0x80000000;
}

void matrix_start()
{
    matrix_tetromino_spawn(preview_get());
}
