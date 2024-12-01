#include "data.h"
#include "timer.h"
#include "helper.h"
#include "output.h"

#define TIME_GAP_MAX 1

unsigned int score;
unsigned int level;
unsigned int lines;

unsigned short time_gap;
unsigned int time_cur;
unsigned int time_last;

signed char is_game_over;

void score_add(unsigned int value)
{
    if (value != 0)
    {
        score += value;
        draw_score();
    }
}

void level_add(unsigned int value)
{
    if (value != 0)
    {
        level += value;
        draw_level();
    }
}

void lines_add(unsigned int value)
{
    if (value != 0)
    {
        if (lines / 10 < (lines + value) / 10)
            level_add(1);

        lines += value;
        draw_lines();
    }
}

void data_init()
{
    score = 0;
    level = 1;
    lines = 0;

    is_game_over = 0;

    draw_score();
    draw_level();
    draw_lines();
}

void time_init()
{
    time_cur = 0;
    time_gap = 0;
    time_last = 0;
}

void time_update()
{
    time_cur = _timer;
    time_gap = min(time_cur - time_last, TIME_GAP_MAX);
    time_last = time_cur;
}
