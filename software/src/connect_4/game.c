#include <stdbool.h>
#include <stdint.h>

#include "game.h"
#include "data.h"

int32_t bitmap_insert(Bitmap *player, Bitmap *competitor, int32_t x)
{
    for (int32_t y = HIGHTH - 1; y >= 0; y--)
        if (player->data[x][y] == false && competitor->data[x][y] == false)
            return y;

    return -1;
}

bool bitmap_get(Bitmap *bitmap, int32_t x, int32_t y)
{
    return (x >= 0 && y >= 0 && x < WIDTH && y < HIGHTH) ? bitmap->data[x][y] : false;
}

bool bitmap_is_full(Bitmap *player, Bitmap *competitor)
{
    bool value = true;
    for (int32_t x = 0; x < WIDTH; x++)
        value = value && (player->data[x][0] || competitor->data[x][0]);
    return value;
}

bool bitmap_is_connect(Bitmap *bitmap, int32_t x, int32_t y)
{
    const Point dirs[4] = {{+1, +0}, {+0, +1}, {+1, +1}, {+1, -1}};

    for (int32_t d = 0; d < 4; d++)
    {
        int32_t c = 1;

        for (int32_t i = 1; i < CONNECTION; i++)
            if (bitmap_get(bitmap, x + dirs[d].x * i, y + dirs[d].y * i))
                c++;
            else
                break;

        for (int32_t i = -1; i > -CONNECTION; i--)
            if (bitmap_get(bitmap, x + dirs[d].x * i, y + dirs[d].y * i))
                c++;
            else
                break;

        if (c >= CONNECTION)
            return true;
    }

    return false;
}

bool bitmap_is_connect_pos(Bitmap *bitmap, int32_t x, int32_t y, Point *pos)
{
    const Point dirs[4] = {{+1, +0}, {+0, +1}, {+1, +1}, {+1, -1}};

    for (int32_t d = 0; d < 4; d++)
    {
        int32_t c = 1;

        pos[0].x = x;
        pos[0].y = y;

        for (int32_t i = 1; i < CONNECTION; i++)
            if (bitmap_get(bitmap, x + dirs[d].x * i, y + dirs[d].y * i))
            {
                pos[c].x = x + dirs[d].x * i;
                pos[c].y = y + dirs[d].y * i;
                c++;
            }
            else
                break;

        for (int32_t i = -1; i > -CONNECTION; i--)
            if (bitmap_get(bitmap, x + dirs[d].x * i, y + dirs[d].y * i))
            {
                pos[c].x = x + dirs[d].x * i;
                pos[c].y = y + dirs[d].y * i;
                c++;
            }
            else
                break;

        if (c >= CONNECTION)
            return true;
    }

    return false;
}

void bitmap_init(Bitmap *bitmap)
{
    for (int32_t x = 0; x < WIDTH; x++)
        for (int32_t y = 0; y < HIGHTH; y++)
            bitmap->data[x][y] = 0;
}

void player_init(Player *player, void (*draw_name)(), bool (*move)(Player *player, Player *competitor), bool color)
{
    player->x = WIDTH / 2;
    player->draw_name = draw_name;
    player->move = move;
    player->color = color;

    bitmap_init(&player->bitmap);
}

bool player_is_connect(Player *player, Point *pos)
{
    return bitmap_is_connect_pos(&player->bitmap, player->x, player->y, pos);
}

bool player_insert(Player *player, Player *competitor)
{
    player->y = bitmap_insert(&player->bitmap, &competitor->bitmap, player->x);
    if (player->y >= 0)
    {
        player->bitmap.data[player->x][player->y] = true;
        return true;
    }
    else
        return false;
}
