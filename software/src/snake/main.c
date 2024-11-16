#include "display_96_64.h"
#include "random_number_generator.h"
#include "five_way_motion_sensor.h"
#include "driver.h"

#include "snake.h"

#define GRID_SIZE 5
#define GRID_OFFSET_X 2
#define GRID_OFFSET_Y 2

#define GAME_SIZE_X 16
#define GAME_SIZE_Y 12
#define GAME_PIXEL_SIZE_X (GAME_SIZE_X * GRID_SIZE + 2 * GRID_OFFSET_X)
#define GAME_PIXEL_SIZE_Y (GAME_SIZE_Y * GRID_SIZE + 2 * GRID_OFFSET_Y)

#define SCORE_POS_X (DISPLAY_SIZE_X - 9)
#define SCORE_POS_Y 2
#define TIME_POS_X (DISPLAY_SIZE_X - 9)
#define TIME_POS_Y (DISPLAY_SIZE_Y - 3)

#define SNAKE_DEC_U 0
#define SNAKE_DEC_D 1
#define SNAKE_DEC_R 2
#define SNAKE_DEC_L 3

#define SNAKE_FOOD_SCORE 1
#define SNAKE_BIG_FOOD_SCORE 4
#define SNAKE_INIT_LENGTH 4
#define SNAKE_MAX_LENGTH (GAME_SIZE_X * GAME_SIZE_Y)
#define FOOD_MAX_TIME 32
#define FOOD_BIG_PROB 4

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
    unsigned char big;
    short time;
} Food;

typedef struct
{
    short num;
} Time;

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

typedef struct
{
    short num;
    char *str;
} Score;

char *itoa(int value, char *str, int base)
{
    char *ret = str;
    if (value == 0)
    {
        *str++ = '0';
    }
    else
    {
        int sum;

        if (base == 10 && value < 0)
        {
            value = -value;
            *str++ = '-';
        }

        while (value != 0)
        {
            sum = value % base;
            *str++ = (sum > 9) ? (char)(sum - 10) + 'a' : (char)sum + '0';
            value /= base;
        }

        for (char *left = ret, *right = str - 1; left < right; left++, right--)
        {
            char tmp = *left;
            *left = *right;
            *right = tmp;
        }
    }
    *str = '\0';
    return ret;
}

void split_line_init()
{
    draw_line(GAME_PIXEL_SIZE_X - 1, 0, GAME_PIXEL_SIZE_X - 1, GAME_PIXEL_SIZE_Y - 1, solid_line_one_func);
}

void score_update(Score *sc)
{
    itoa((int)sc->num, sc->str, 10);
    int i = 0, y = 0;
    while (sc->str[i] != '\0')
    {
        draw_grid_char(SCORE_POS_X, SCORE_POS_Y + y, 5, 7, number_grid[sc->str[i++] - '0']);
        y += 9;
    }
}

void score_init(Score *sc, char *str)
{
    sc->num = 0;
    sc->str = str;
    score_update(sc);
}

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
    draw_grid_char((int)x * GRID_SIZE + GRID_OFFSET_X, (int)y * GRID_SIZE + GRID_OFFSET_Y, GRID_SIZE, GRID_SIZE, grid);
}

void draw_big_grid_helper(signed char x, signed char y, unsigned char *grid)
{
    draw_grid_char((int)x * GRID_SIZE + GRID_OFFSET_X - 1, (int)y * GRID_SIZE + GRID_OFFSET_Y - 1, GRID_SIZE + 2, GRID_SIZE + 2, grid);
}

void erase_grid(signed char x, signed char y)
{
    draw_grid_helper(x, y, (unsigned char *)zero_grid);
}

void erase_big_grid(signed char x, signed char y)
{
    draw_big_grid_helper(x, y, (unsigned char *)zero_grid);
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

void draw_food(Food *f)
{
    if (f->big)
        draw_big_grid_helper(f->x, f->y, (unsigned char *)big_food_grid[f->time & 1]);
    else
        draw_grid_helper(f->x, f->y, (unsigned char *)food_grid[f->time & 1]);
}

void time_update(Time *t, short target_time)
{
    target_time = FOOD_MAX_TIME - target_time;
    if (t->num <= target_time)
        for (int i = t->num; i < target_time; i++)
        {
            draw_pixel(TIME_POS_X + 2, TIME_POS_Y - i - 3, 1);
            draw_pixel(TIME_POS_X + 3, TIME_POS_Y - i - 3, 1);
        }

    else
        for (int i = t->num; i > target_time; i--)
        {
            draw_pixel(TIME_POS_X + 2, TIME_POS_Y - i - 3, 0);
            draw_pixel(TIME_POS_X + 3, TIME_POS_Y - i - 3, 0);
        }
    t->num = target_time;
}

void time_init(Time *t, short target_time)
{
    t->num = 0;
    draw_rect(TIME_POS_X, TIME_POS_Y - (FOOD_MAX_TIME + 4), 5, FOOD_MAX_TIME + 3, solid_line_one_func);
    time_update(t, target_time);
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

signed char food_init(Food *f, Bitmap *h, Time *t, unsigned char x, unsigned char y, unsigned char big)
{
    if (!bitmap_get_bit(h, x, y))
    {
        f->x = x;
        f->y = y;
        f->big = big;
        f->time = 0;
        bitmap_set_bit(h, f->x, f->y, 1);
        draw_food(f);
        time_update(t, f->time);
        return 1;
    }
    else
        return 0;
}

signed char food_rand_init(Food *f, Bitmap *h, Time *t)
{
    unsigned char x = rand.u8 % GAME_SIZE_X;
    unsigned char y = rand.u8 % GAME_SIZE_Y;
    unsigned char b = (rand.u8 & (FOOD_BIG_PROB - 1)) == 0;
    for (int i = 0; i < SNAKE_MAX_LENGTH; i++)
    {
        if (food_init(f, h, t, x, y, b))
            return 1;
        x = (x + i * ((GAME_SIZE_X / 2) + 1)) % GAME_SIZE_X;
        y = (y + i * ((GAME_SIZE_Y / 2) + 1)) % GAME_SIZE_Y;
    }
    return 0;
}

void food_update(Food *f, Bitmap *h, Time *t)
{
    f->time++;
    if (f->big)
    {
        if (f->time > FOOD_MAX_TIME)
        {
            bitmap_set_bit(h, f->x, f->y, 0);
            erase_big_grid(f->x, f->y);
            food_rand_init(f, h, t);
        }
        else
            time_update(t, f->time);
    }
    draw_food(f);
}

// void bitmap_init(Bitmap *h) __attribute__((optimize("O0")));     // nostdlib
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

signed char update(Bitmap *h, Queue *q, Snake *s, Food *f, Score *sc, Time *t)
{
    char save_x = s->head_x, save_y = s->head_y;
    dec_to_pos(s->d, &s->head_x, &s->head_y);
    if (bitmap_get_bit(h, s->head_x, s->head_y))
    {
        if (s->head_x == f->x && s->head_y == f->y)
        {
            if (f->big)
            {
                s->food += SNAKE_BIG_FOOD_SCORE;
                sc->num += SNAKE_BIG_FOOD_SCORE;
                erase_big_grid(f->x, f->y);
            }
            else
            {
                s->food += SNAKE_FOOD_SCORE;
                sc->num += SNAKE_FOOD_SCORE;
            }
            score_update(sc);
            food_rand_init(f, h, t);
        }
        else
            return 1;
    }
    food_update(f, h, t);
    bitmap_set_bit(h, s->head_x, s->head_y, 1);
    draw_snake_body(save_x, save_y, s->d, queue_peek_first(q));
    queue_push(q, s->d);
    draw_snake_head(s->head_x, s->head_y, s->d);
    if (s->food > 0)
    {
        s->food--;
    }
    else
    {
        bitmap_set_bit(h, s->tail_x, s->tail_y, 0);
        erase_grid(s->tail_x, s->tail_y);
        dec_to_pos(queue_pop(q), &s->tail_x, &s->tail_y);
    }
    draw_snake_tail(s->tail_x, s->tail_y, queue_peek_last(q));
    return 0;
}

void game_end_fail()
{
    draw_grid_int(DISPLAY_SIZE_X / 2, DISPLAY_SIZE_Y / 2 - 5, 28, 11, game_over_grid[0]);
    draw_grid_int(DISPLAY_SIZE_X / 2 - 28, DISPLAY_SIZE_Y / 2 - 5, 28, 11, game_over_grid[1]);
    display_refresh();
}

void game_end_victory()
{
    draw_grid_int(DISPLAY_SIZE_X / 2, DISPLAY_SIZE_Y / 2 - 5, 28, 11, your_win_grid[0]);
    draw_grid_int(DISPLAY_SIZE_X / 2 - 28, DISPLAY_SIZE_Y / 2 - 5, 28, 11, your_win_grid[1]);
    display_refresh();
}

int main()
{
    Bitmap bitmap;
    Queue queue;
    Snake snake;
    Food food;
    Score score;
    Time time;

    char score_str[3] = {0};

    while (1)
    {
        display_clear();
        split_line_init();
        time_init(&time, 0);
        bitmap_init(&bitmap);
        queue_init(&queue);
        snake_init(&snake, &bitmap, &queue, GAME_SIZE_X / 2, GAME_SIZE_Y / 2, 0, SNAKE_INIT_LENGTH);
        food_rand_init(&food, &bitmap, &time);
        score_init(&score, score_str);

        while (1)
        {
            player_input(&snake);
            if (update(&bitmap, &queue, &snake, &food, &score, &time))
            {
                game_end_fail();
                break;
            }
            if (snake.length >= SNAKE_MAX_LENGTH)
            {
                game_end_victory();
                break;
            }
            display_refresh();
        }

        driver_off();
    }
}
