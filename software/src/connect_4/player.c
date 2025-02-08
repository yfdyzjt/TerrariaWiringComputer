#include <stdbool.h>

#include "player.h"
#include "input.h"
#include "output.h"
#include "data.h"
#include "game.h"

bool player_move(Player *player, Player *competitor)
{
    player->y = -1;
    while (true)
    {
        draw_player_erase_piece(player);
        key_refresh();
        if (key_j() && player_insert(player, competitor))
        {
            draw_player_piece(player);
            break;
        }
        if (key_r() && player->x - 1 >= 0)
            player->x--;
        if (key_l() && player->x + 1 < WIDTH)
            player->x++;
        draw_player_piece(player);
    }
    return true;
}
