#include "display_96_64.h"
#include "random_number_generator.h"
#include "five_way_motion_sensor.h"
#include "driver.h"

#include "snake_grid.h"

#define GRID_SIZE 5
#define GRID_OFFSET_X 2
#define GRID_OFFSET_Y 2

#define GAME_SIZE_X 16
#define GAME_SIZE_Y 12

#define SNAKE_MAX_LENGTH (GAME_SIZE_X * GAME_SIZE_Y)
#define SNAKE_INIT_LENGTH 4
#define SNAKE_DEC_U 0
#define SNAKE_DEC_D 1
#define SNAKE_DEC_R 2
#define SNAKE_DEC_L 3

const unsigned char snake_body_link_map[16] = {0, 0, 1, 2, 0, 0, 3, 4, 4, 2, 5, 0, 3, 1, 0, 5};

typedef struct
{
    unsigned char d;
    signed char head_x, head_y;
    signed char tail_x, tail_y;
    signed char food;
    short length;
} Snake;

typedef struct
{
    signed char x, y;
} Food;

typedef struct
{
    unsigned char data[GAME_SIZE_X / 8][GAME_SIZE_Y];
} Bitmap;

typedef struct
{
    unsigned char data[SNAKE_MAX_LENGTH / 4];
    short front;
    short rear;
} Queue;

unsigned char bitmap_get_bit(Bitmap *h, signed char x, signed char y)
{
    return (h->data[(unsigned char)x / 8][(unsigned char)y] >> ((unsigned char)x % 8)) & 1;
}

void bitmap_set_bit(Bitmap *h, signed char x, signed char y, unsigned char b)
{
    unsigned char offset = x % 8;
    unsigned char *addr = &h->data[(unsigned char)x / 8][(unsigned char)y];
    *addr = b ? *addr | (1 << offset) : *addr & ~(1 << offset);
}

void draw_grid_helper(signed char x, signed char y, unsigned char *grid)
{
    draw_grid((int)x * GRID_SIZE + GRID_OFFSET_X, (int)y * GRID_SIZE + GRID_OFFSET_Y, GRID_SIZE, GRID_SIZE, grid);
}

void erase_grid(signed char x, signed char y)
{
    draw_grid_helper(x, y, (unsigned char *)zero_grid);
}

void draw_snake_head(signed char x, signed char y, unsigned char d)
{
    draw_grid_helper(x, y, (unsigned char *)head_grid[d]);
}

void draw_snake_body(signed char x, signed char y, unsigned char d, unsigned char od)
{
    draw_grid_helper(x, y, (unsigned char *)body_grid[snake_body_link_map[(od << 2) ^ d]][(x ^ y) & 1]);
}

void draw_snake_tail(signed char x, signed char y, unsigned char d)
{
    draw_grid_helper(x, y, (unsigned char *)tail_grid[d][(x ^ y) & 1]);
}

void draw_food(signed char x, signed char y)
{
    draw_grid_helper(x, y, (unsigned char *)food_grid);
}

void snake_init(Snake *s, Bitmap *h, Queue *q, signed char x, signed char y, unsigned char d, char l)
{
    s->head_x = x;
    s->head_y = y;
    s->tail_x = x;
    s->tail_y = y;
    s->food = l;
    s->d = d;
    s->length = 0;
    bitmap_set_bit(h, x, y, 1);
}

signed char food_init(Food *f, Bitmap *h, unsigned char x, unsigned char y)
{
    if (!bitmap_get_bit(h, x, y))
    {
        f->x = x;
        f->y = y;
        bitmap_set_bit(h, f->x, f->y, 1);
        draw_food(f->x, f->y);
        return 1;
    }
    else
        return 0;
}

signed char food_rand_init(Food *f, Bitmap *h)
{
    unsigned char x = (unsigned char)_random_number % GAME_SIZE_X;
    unsigned char y = (unsigned char)_random_number % GAME_SIZE_Y;
    for (int i = 0; i < SNAKE_MAX_LENGTH; i++)
    {
        if (food_init(f, h, x, y))
            return 1;
        x = (x + i * ((GAME_SIZE_X / 2) + 1)) % GAME_SIZE_X;
        y = (y + i * ((GAME_SIZE_Y / 2) + 1)) % GAME_SIZE_Y;
    }
    return 0;
}

void bitmap_init(Bitmap *h) __attribute__((optimize("O0")));
void bitmap_init(Bitmap *h)
{
    for (int i = 0; i < GAME_SIZE_X / 8; i++)
    {
        for (int j = 0; j < GAME_SIZE_Y; j++)
        {
            h->data[i][j] = 0;
        }
    }
}

void queue_init(Queue *q)
{
    q->front = 0;
    q->rear = 0;
}

unsigned char queue_peek_first(Queue *q)
{
    return (q->data[(q->rear - 1) >> 2] >> (((q->rear - 1) & 3) << 1)) & 3;
}

unsigned char queue_peek_last(Queue *q)
{
    return (q->data[q->front >> 2] >> ((q->front & 3) << 1)) & 3;
}

void queue_push(Queue *q, unsigned char data)
{
    unsigned char offset = (q->rear & 3) << 1;
    q->data[q->rear >> 2] &= ~(3 << offset);
    q->data[q->rear >> 2] ^= data << offset;
    q->rear = (q->rear + 1) % SNAKE_MAX_LENGTH;
}

unsigned char queue_pop(Queue *q)
{
    unsigned char data = queue_peek_last(q);
    q->front = (q->front + 1) % SNAKE_MAX_LENGTH;
    return data;
}

void dec_to_pos(unsigned char d, signed char *x, signed char *y)
{
    switch (d)
    {
    case SNAKE_DEC_L:
        (*x)++;
        break;
    case SNAKE_DEC_R:
        (*x)--;
        break;
    case SNAKE_DEC_D:
        (*y)++;
        break;
    case SNAKE_DEC_U:
        (*y)--;
        break;
    }
    if (*x < 0)
        *x += GAME_SIZE_X;
    else if (*x >= GAME_SIZE_X)
        *x -= GAME_SIZE_X;
    if (*y < 0)
        *y += GAME_SIZE_Y;
    else if (*y >= GAME_SIZE_Y)
        *y -= GAME_SIZE_Y;
}

void player_input(Snake *s)
{
    unsigned char buffer = _motion_sensor_buffer;
    if (buffer & MOTION_SENSOR_U && s->d != SNAKE_DEC_D)
        s->d = SNAKE_DEC_U;
    else if (buffer & MOTION_SENSOR_D && s->d != SNAKE_DEC_U)
        s->d = SNAKE_DEC_D;
    else if (buffer & MOTION_SENSOR_R && s->d != SNAKE_DEC_L)
        s->d = SNAKE_DEC_R;
    else if (buffer & MOTION_SENSOR_L && s->d != SNAKE_DEC_R)
        s->d = SNAKE_DEC_L;
}

signed char update(Bitmap *h, Queue *q, Snake *s, Food *f)
{
    draw_snake_body(s->head_x, s->head_y, s->d, queue_peek_first(q));
    dec_to_pos(s->d, &s->head_x, &s->head_y);
    if (bitmap_get_bit(h, s->head_x, s->head_y))
    {
        if (s->head_x == f->x && s->head_y == f->y)
        {
            s->food++;
            food_rand_init(f, h);
        }
        else
        {
            return 1;
        }
    }
    bitmap_set_bit(h, s->head_x, s->head_y, 1);
    queue_push(q, s->d);
    draw_snake_head(s->head_x, s->head_y, s->d);
    if (s->food > 0)
        s->food--;
    else
    {
        bitmap_set_bit(h, s->tail_x, s->tail_y, 0);
        erase_grid(s->tail_x, s->tail_y);
        dec_to_pos(queue_pop(q), &s->tail_x, &s->tail_y);
    }
    draw_snake_tail(s->tail_x, s->tail_y, queue_peek_last(q));
    return 0;
}

int main()
{
    Bitmap bitmap;
    Queue queue;
    Snake snake;
    Food food;

    while (1)
    {
        display_clear();
        bitmap_init(&bitmap);
        queue_init(&queue);
        snake_init(&snake, &bitmap, &queue, GAME_SIZE_X / 2, GAME_SIZE_Y / 2, 0, SNAKE_INIT_LENGTH);
        food_rand_init(&food, &bitmap);

        while (1)
        {
            player_input(&snake);
            if (update(&bitmap, &queue, &snake, &food))
            {
                break;
            }
            if (snake.length >= SNAKE_MAX_LENGTH)
            {
                break;
            }
            display_refresh();
        }

        driver_off();
    }
}
