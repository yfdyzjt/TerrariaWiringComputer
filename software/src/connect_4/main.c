#include <stdbool.h>
#include <stdint.h>

#include "output.h"
#include "input.h"
#include "player.h"
#include "computer.h"
#include "game.h"
#include "data.h"

Player player, computer;
Player *players[2];

void game_init();
void game_loop();
void game_pause();

int main()
{
    while (true)
    {
        game_init();
        game_loop();
        game_pause();
    }
}

void game_init()
{
    bool rand_color = (bool)(random_number_get() & 1);

    player_init(&player, draw_player_text, player_move, rand_color);
    player_init(&computer, draw_computer_text, computer_move, !rand_color);

    players[0] = rand_color ? &player : &computer;
    players[1] = rand_color ? &computer : &player;

    draw_init();
}

void game_loop()
{
    while (true)
    {
        Point pos[CONNECTION];
        for (int32_t i = 0; i < 2; i++)
        {
            players[i]->draw_name();
            if (bitmap_is_full(&players[i]->bitmap, &players[i ^ 1]->bitmap) ||
                !players[i]->move(players[i], players[i ^ 1]))
            {
                draw_nobody_text(pos);
                draw_win_text();
                return;
            }
            draw_player_piece(players[i]);
            if (player_is_connect(players[i], pos))
            {
                draw_win_piece(pos);
                draw_win_text();
                return;
            }
        }
    }
}

void game_pause()
{
    key_wait();
}
