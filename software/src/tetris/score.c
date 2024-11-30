#include "score.h"
#include "data.h"

signed char is_b2b;
signed char is_t_spin;
signed char is_mini_t_spin;
signed char is_perfect_clear;

signed char combo;

void score_soft_drop()
{
    score_add(1);
}

void score_hard_drop(signed char dy)
{
    score_add(dy * 2);
}

void score_combo()
{
    score_add(50 * combo * level);
}

void score_high_line_clear(unsigned short score)
{
    if (is_b2b)
        score_add(score * 3 / 2 * level);
    else
        score_add(score * level);
    score_combo();

    is_b2b = 1;
    combo += 1;
}

void score_low_line_clear(unsigned short score)
{
    score_add(score * level);
    score_combo();

    is_b2b = 0;
    combo += 1;
}

void score_line_no_clear(unsigned short score)
{
    score_add(score * level);

    combo = 0;
}

void score_single()
{
    score_low_line_clear(100);
}

void score_double()
{
    score_low_line_clear(300);
}

void score_triple()
{
    score_low_line_clear(500);
}

void score_tetris()
{
    score_high_line_clear(800);
}

void score_mini_t_spin()
{
    score_line_no_clear(100);
}

void score_t_spin()
{
    score_line_no_clear(400);
}

void score_mini_t_spin_single()
{
    score_high_line_clear(200);
}

void score_t_spin_single()
{
    score_high_line_clear(800);
}

void score_mini_t_spin_double()
{
    score_high_line_clear(1200);
}

void score_t_spin_double()
{
    score_high_line_clear(1200);
}

void score_t_spin_triple()
{
    score_high_line_clear(1600);
}

void score_perfect_clear_single()
{
    score_low_line_clear(800);
}

void score_perfect_clear_double()
{
    score_low_line_clear(1200);
}

void score_perfect_clear_triple()
{
    score_low_line_clear(1200);
}

void score_perfect_clear_tetris()
{
    score_high_line_clear(2000);
}

void score_lock(unsigned char clear_lines)
{

    if (is_perfect_clear)
        switch (clear_lines)
        {
        case 1:
            score_perfect_clear_single();
            break;
        case 2:
            score_perfect_clear_double();
            break;
        case 3:
            score_perfect_clear_triple();
            break;
        case 4:
            score_perfect_clear_tetris();
            break;
        }
    else if (is_t_spin)
        switch (clear_lines)
        {
        case 0:
            score_mini_t_spin();
            break;
        case 1:
            score_mini_t_spin_single();
            break;
        case 2:
            score_mini_t_spin_double();
            break;
        }
    else if (is_t_spin)
        switch (clear_lines)
        {
        case 0:
            score_t_spin();
            break;
        case 1:
            score_t_spin_single();
            break;
        case 2:
            score_t_spin_double();
            break;
        case 3:
            score_t_spin_triple();
            break;
        }
    else
        switch (clear_lines)
        {
        case 0:
            score_line_no_clear(0);
            break;
        case 1:
            score_single();
            break;
        case 2:
            score_double();
            break;
        case 3:
            score_triple();
            break;
        case 4:
            score_tetris();
            break;
        }

    is_perfect_clear = 0;
    is_mini_t_spin = 0;
    is_t_spin = 0;
}

void score_init()
{
    is_perfect_clear = 0;
    is_mini_t_spin = 0;
    is_t_spin = 0;
    is_b2b = 0;

    combo = 0;
}
