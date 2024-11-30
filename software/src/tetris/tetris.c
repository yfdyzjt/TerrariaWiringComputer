#include "matrix.h"
#include "preview.h"
#include "hold.h"
#include "output.h"
#include "input.h"
#include "bag.h"
#include "drop.h"
#include "data.h"
#include "score.h"

void game_init();
void game_start();
void game_loop();

int main()
{

    while (1)
    {
        game_init();
        game_start();
        game_loop();

        interrupt();
    }

    return 0;
}

void game_init()
{
    draw_clear();
    draw_ui();

    data_init();
    time_init();
    bag_init();
    hold_init();
    matrix_init();
    score_init();

    draw_refresh();
}

void game_start()
{
    preview_start();
    matrix_start();

    draw_refresh();
}

void game_loop()
{
    while (1)
    {
        time_update();
        input_update();
        drop_update();

        draw_refresh();
    }
}
