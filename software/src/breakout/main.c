#define FIXED_POINT_SHIFT 16

#include "random_number_generator.h"
#include "five_way_motion_sensor.h"
#include "display_96_64.h"
#include "driver.h"
#include "fixed_number.h"

#define PADDLE_WIDTH 16
#define PADDLE_HEIGHT 2
#define PADDLE_ANGLE 4

#define BALL_SIZE 2
#define BALL_VELOCITY_FACTOR FLOAT_TO_FIXED(1.1)

#define BRICK_WIDTH 4
#define BRICK_HEIGHT 2
#define BRICK_NUMBER_X (DISPLAY_SIZE_X / BRICK_WIDTH)
#define BRICK_NUMBER_Y 8
#define BRICK_BEGIN_Y 9
#define BRICK_END_Y BRICK_BEGIN_Y + (BRICK_HEIGHT + 1) * BRICK_NUMBER_Y

#define MAX_LIVES 6

const fixed angle_table[PADDLE_ANGLE][2] = {
    {FLOAT_TO_FIXED(0.8660), FLOAT_TO_FIXED(0.5000)},
    {FLOAT_TO_FIXED(0.7071), FLOAT_TO_FIXED(0.7071)},
    {FLOAT_TO_FIXED(0.5000), FLOAT_TO_FIXED(0.8660)},
    {FLOAT_TO_FIXED(0.2588), FLOAT_TO_FIXED(0.9659)},
};

const unsigned char brick_grid[BRICK_HEIGHT] = {0b1111, 0b1111};
const unsigned char brick_inv_grid[BRICK_HEIGHT] = {0};
const unsigned short paddle_grid[PADDLE_HEIGHT] = {0b1111111111111111, 0b1111111111111111};
const unsigned short paddle_inv_grid[PADDLE_HEIGHT] = {0};

typedef struct
{
    fixed x, y;
    fixed dx, dy;
    fixed v;
    int o;
} Ball;

typedef struct
{
    int x, y;
    int width;
} Paddle;

typedef struct
{
    int active;
    int score;
} Brick;

Ball ball;
Paddle paddle;
Brick bricks[BRICK_NUMBER_X][BRICK_NUMBER_Y];

int score;
int lives;
int hit_ball;

char score_str[3];
char lives_str[3];

int sign(int value)
{
    return value >= 0 ? 1 : -1;
}

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

unsigned char get_pixel(int x, int y)
{
    if (x >= 0 && x < DISPLAY_SIZE_X && y >= 0 && y < DISPLAY_SIZE_Y)
        return (_display_buffer[x / 8 + y * 16] >> (x % 8)) & 1;
    else
        return 0;
}

void draw_ball()
{
    if (get_pixel(FIXED_TO_INT(ball.x), FIXED_TO_INT(ball.y)))
        ball.o = 1;
    else
    {
        ball.o = 0;
        draw_pixel(FIXED_TO_INT(ball.x), FIXED_TO_INT(ball.y), 1);
    }
}

void erase_ball()
{
    if (!ball.o)
        draw_pixel(FIXED_TO_INT(ball.x), FIXED_TO_INT(ball.y), 0);
}

void draw_brick(int x, int y)
{
    draw_grid_char(x * (BRICK_WIDTH + 1) + 1,
                   y * (BRICK_HEIGHT + 1) + BRICK_BEGIN_Y,
                   BRICK_WIDTH,
                   BRICK_HEIGHT,
                   (unsigned char *)brick_grid);
}

void erase_brick(int x, int y)
{
    draw_grid_char(x * (BRICK_WIDTH + 1) + 1,
                   y * (BRICK_HEIGHT + 1) + BRICK_BEGIN_Y,
                   BRICK_WIDTH,
                   BRICK_HEIGHT,
                   (unsigned char *)brick_inv_grid);
}

void draw_paddle()
{
    draw_grid_short(paddle.x,
                    paddle.y,
                    paddle.width,
                    PADDLE_HEIGHT,
                    (unsigned short *)paddle_grid);
}

void erase_paddle()
{
    draw_grid_short(paddle.x,
                    paddle.y,
                    paddle.width,
                    PADDLE_HEIGHT,
                    (unsigned short *)paddle_inv_grid);
}

void init_brick(int x, int y)
{
    bricks[x][y].active = 1;
    bricks[x][y].score = 1 + (BRICK_NUMBER_Y - y - 1) / 2 * 2;
    draw_brick(x, y);
}

void init_ball(fixed x, fixed y, fixed dx, fixed dy, fixed v)
{
    erase_ball();
    ball.x = x;
    ball.y = y;
    ball.dx = dx;
    ball.dy = dy;
    ball.v = v;
    ball.o = 0;
    draw_ball();
}

void init_rand_ball()
{
    init_ball(FLOAT_TO_FIXED(paddle.x + rand.u32 % paddle.width),
              FLOAT_TO_FIXED(DISPLAY_SIZE_Y - 7),
              FLOAT_TO_FIXED(0),
              FLOAT_TO_FIXED(1),
              FLOAT_TO_FIXED(1));
}

void init_paddle()
{
    erase_paddle();
    paddle.x = (DISPLAY_SIZE_X - PADDLE_WIDTH) / 2;
    paddle.y = DISPLAY_SIZE_Y - PADDLE_HEIGHT - 4;
    paddle.width = PADDLE_WIDTH;
    draw_paddle();
}

void draw_score()
{
    itoa(score, score_str, 10);
    draw_string(DISPLAY_SIZE_X, 1, score_str);
}

void draw_lives()
{
    itoa(lives, lives_str, 10);
    draw_string(7, 1, lives_str);
}

void init_game()
{
    hit_ball = 0;
    score = 0;
    lives = MAX_LIVES;

    draw_lives();
    draw_score();

    init_paddle();
    init_rand_ball();

    for (int i = 0; i < BRICK_NUMBER_X; i++)
        for (int j = 0; j < BRICK_NUMBER_Y; j++)
            init_brick(i, j);
}

void update_ball()
{
    erase_ball();

    fixed v = ball.v;
    while (v > 0)
    {
        ball.x += FIXED_MUL(ball.dx, v >= FLOAT_TO_FIXED(1) ? FLOAT_TO_FIXED(1) : v);
        ball.y += FIXED_MUL(ball.dy, v >= FLOAT_TO_FIXED(1) ? FLOAT_TO_FIXED(1) : v);
        v -= FLOAT_TO_FIXED(1);
        if (v < 0)
            break;

        if (FIXED_TO_INT(ball.x) <= 0 || FIXED_TO_INT(ball.x) >= DISPLAY_SIZE_X - 1)
            ball.dx = -ball.dx;

        if (FIXED_TO_INT(ball.y) <= 0)
        {
            ball.dy = -ball.dy;
            ball.y = 0 - ball.y + FLOAT_TO_FIXED(1);

            if (paddle.width == PADDLE_WIDTH)
            {
                erase_paddle();
                paddle.width /= 2;
                paddle.x += PADDLE_WIDTH / 4;
                draw_paddle();
            }
        }
        else if (FIXED_TO_INT(ball.y) >= DISPLAY_SIZE_Y)
        {
            lives--;
            draw_lives();

            if (lives > 0)
                init_rand_ball();

            return;
        }

        if (FIXED_TO_INT(ball.y) >= paddle.y &&
            FIXED_TO_INT(ball.y) < paddle.y + PADDLE_HEIGHT &&
            FIXED_TO_INT(ball.x) >= paddle.x &&
            FIXED_TO_INT(ball.x) < paddle.x + paddle.width)
        {
            int hit_area = (FIXED_TO_INT(ball.x) - paddle.x) / (paddle.width / (PADDLE_ANGLE * 2));
            ball.y = FLOAT_TO_FIXED(paddle.y << 1) - ball.y - FLOAT_TO_FIXED(1);
            if (hit_area >= PADDLE_ANGLE)
            {
                hit_area = (PADDLE_ANGLE * 2 - 1) - hit_area;
                ball.dx = angle_table[hit_area][0];
                ball.dy = -angle_table[hit_area][1];
            }
            else
            {
                ball.dx = -angle_table[hit_area][0];
                ball.dy = -angle_table[hit_area][1];
            }

            hit_ball++;
            if (hit_ball == 4 || hit_ball == 12)
                ball.v = FIXED_MUL(ball.v, BALL_VELOCITY_FACTOR);
        }

        if (FIXED_TO_INT(ball.y) >= BRICK_BEGIN_Y &&
            FIXED_TO_INT(ball.y) < BRICK_END_Y &&
            FIXED_TO_INT(ball.x) % 5 != 0 &&
            FIXED_TO_INT(ball.y) % 3 != 0 &&
            bricks[FIXED_TO_INT(ball.x) / 5][(FIXED_TO_INT(ball.y) - BRICK_BEGIN_Y) / 3].active != 0)
        {
            bricks[FIXED_TO_INT(ball.x) / 5][(FIXED_TO_INT(ball.y) - BRICK_BEGIN_Y) / 3].active = 0;
            erase_brick(FIXED_TO_INT(ball.x) / 5, (FIXED_TO_INT(ball.y) - BRICK_BEGIN_Y) / 3);

            score += bricks[FIXED_TO_INT(ball.x) / 5][(FIXED_TO_INT(ball.y) - BRICK_BEGIN_Y) / 3].score;
            draw_score();

            int random_sign = rand.u8 & 1 ? 1 : -1;
            unsigned char random_angle = rand.u8 % PADDLE_ANGLE;
            ball.dx = random_sign * sign(ball.dx) * angle_table[random_angle][0];
            ball.dy = -random_sign * sign(ball.dy) * angle_table[random_angle][1];

            if ((FIXED_TO_INT(ball.y) - BRICK_BEGIN_Y) / 3 >= BRICK_NUMBER_Y / 2)
                ball.v = FIXED_MUL(ball.v, BALL_VELOCITY_FACTOR);
        }
    }

    draw_ball();
}

void move_paddle()
{
    erase_paddle();
    motion_sensor_refresh();
    if (motion_sensor_l())
    {
        paddle.x++;
        if (paddle.x > DISPLAY_SIZE_X - PADDLE_WIDTH)
        {
            paddle.x = DISPLAY_SIZE_X - PADDLE_WIDTH;
        }
    }
    else if (motion_sensor_r())
    {
        paddle.x--;
        if (paddle.x < 0)
        {
            paddle.x = 0;
        }
    }
    draw_paddle();
}

int main()
{
    while (1)
    {
        display_clear();

        init_game();
        while (lives > 0)
        {
            move_paddle();
            update_ball();

            display_refresh();
        }

        driver_off();
    }
    return 0;
}
