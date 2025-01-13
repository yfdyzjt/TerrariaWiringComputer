#include "display_96_64.h"
#include "random_number_generator.h"
#include "five_way_motion_sensor.h"

#define MAZE_WIDTH (DISPLAY_SIZE_X / 2)
#define MAZE_HEIGHT (DISPLAY_SIZE_Y / 2)

#define TABLE_LENGTH (4 * (MAZE_WIDTH + MAZE_HEIGHT))

typedef struct
{
    unsigned char data[MAZE_WIDTH / 8][MAZE_HEIGHT];
} Bitmap;

typedef struct
{
    signed char x, y;
} Point;

typedef struct
{
    Point data[TABLE_LENGTH];
    unsigned short p;
} Table;

const Point dirs[4] = {
    {-2, +0},
    {+0, +2},
    {+2, +0},
    {+0, -2},
};

Bitmap bitmap;
Table table;

char table_pop(Point *point, unsigned short pos)
{
    if (pos < table.p && table.p > 0)
    {
        *point = table.data[pos];

        table.p--;
        table.data[pos] = table.data[table.p];

        return 1;
    }
    else
        return 0;
}

char table_push(Point point)
{
    if (table.p < TABLE_LENGTH)
    {
        table.data[table.p] = point;
        table.p++;

        return 1;
    }
    else
        return 0;
}

char table_random_pop(Point *point)
{
    return table_pop(point, rand.u32 % table.p);
}

void table_init()
{
    table.p = 0;
}

unsigned char bitmap_get(Point point)
{
    return (bitmap.data[(unsigned char)point.x / 8][(unsigned char)point.y] >> ((unsigned char)point.x % 8)) & 1;
}

void bitmap_set(Point point, unsigned char b)
{
    unsigned char offset = point.x % 8;
    unsigned char *addr = &bitmap.data[(unsigned char)point.x / 8][(unsigned char)point.y];
    *addr = b ? *addr | (1 << offset) : *addr & ~(1 << offset);
}

void bitmap_init()
{
    for (int i = 0; i < MAZE_WIDTH / 8; i++)
        for (int j = 0; j < MAZE_HEIGHT; j++)
            bitmap.data[i][j] = 0;
}

void board_set(Point point, unsigned char pixel)
{
    draw_pixel(point.x, point.y, pixel);
    display_refresh();
}

void board_init()
{
    unsigned int value;

    for (int i = 0; i < DISPLAY_SIZE_Y; i++)
    {
        value = i & 1 ? 0xFFFFFFFF : 0xAAAAAAAA;
        *(unsigned int *)&_display_buffer[0 + i * 16] = value;
        *(unsigned int *)&_display_buffer[4 + i * 16] = value;
        *(unsigned int *)&_display_buffer[8 + i * 16] = value;
    }

    display_refresh();
}

unsigned char path_get(Point point)
{
    Point path = {point.x / 2, point.y / 2};
    return bitmap_get(path);
}

void path_set(Point point)
{
    for (int i = 0; i < 4; i++)
    {
        Point new_path = {point.x + dirs[i].x, point.y + dirs[i].y};

        if (new_path.x < 0 || new_path.y < 0 || new_path.x >= MAZE_WIDTH * 2 || new_path.y >= MAZE_HEIGHT * 2)
            continue;

        if (!path_get(new_path))
        {
            Point wall = {point.x + dirs[i].x / 2, point.y + dirs[i].y / 2};
            table_push(wall);
        }
    }

    Point path = {point.x / 2, point.y / 2};
    bitmap_set(path, 1);
}

void wall_get_path(Point wall, Point *path_1, Point *path_2)
{
    if (wall.x & 1)
    {
        path_1->x = wall.x - 1;
        path_1->y = wall.y;
        path_2->x = wall.x + 1;
        path_2->y = wall.y;
    }
    else
    {
        path_1->x = wall.x;
        path_1->y = wall.y - 1;
        path_2->x = wall.x;
        path_2->y = wall.y + 1;
    }
}

int main()
{
    bitmap_init();
    table_init();
    board_init();

    Point start_path = {0, 0};
    path_set(start_path);

    Point wall;
    while (table_random_pop(&wall))
    {
        Point path_1, path_2;

        wall_get_path(wall, &path_1, &path_2);

        if (!path_get(path_1) && path_get(path_2))
        {
            path_set(path_1);
            board_set(wall, 0);
        }
        else if (path_get(path_1) && !path_get(path_2))
        {
            path_set(path_2);
            board_set(wall, 0);
        }
    }

    return 0;
}
