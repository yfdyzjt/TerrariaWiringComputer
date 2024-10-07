#include "five_way_motion_sensor.h"
#include "display_96_64.h"
#include "random_number_generator.h"
#include "driver.h"

#define PADDLE_HEIGHT 8
#define VELOCITY_FACTOR 78643
#define MAX_SCORE 10

const int angleMap[PADDLE_HEIGHT][2] = {{46341, -46341}, {56756, -32768}, {63303, -16962}, {65536, 0}, {65536, 0}, {63303, 16962}, {56756, 32768}, {46341, 46341}};

typedef struct
{
	int x, y;
	int dx, dy;
	int v;
	int o;
} Ball;

typedef struct
{
	int x, y;
} Paddle;

typedef struct
{
	int x, y;
	int num;
} Score;

unsigned char get_pixel(int posX, int posY)
{
	return (_display_buffer[posX / 8 + posY * 16] >> (posX % 8)) & 1;
}

int fixed_point_multiply(int a, int b)
{
	return (int)(((long long)a * (long long)b) >> 16);
}

int get_abs(int a)
{
	return a > 0 ? a : -a;
}

short get_high_bits(int num)
{
	return *((short *)&num + 1);
}

void draw_ball(Ball *ball)
{
	if (get_pixel((int)get_high_bits(ball->x), (int)get_high_bits(ball->y)))
		ball->o = 1;
	else
	{
		ball->o = 0;
		draw_pixel((int)get_high_bits(ball->x), (int)get_high_bits(ball->y), 1);
	}
}

void erase_ball(Ball *ball)
{
	if (!ball->o)
		draw_pixel((int)get_high_bits(ball->x), (int)get_high_bits(ball->y), 0);
}

void draw_paddle(Paddle *paddle)
{
	draw_line(paddle->x, paddle->y, paddle->x, paddle->y + PADDLE_HEIGHT - 1, solid_line_one_func);
}

void draw_paddle_increase(Paddle *paddle, int direction)
{
	if (direction == 1)
	{
		draw_pixel(paddle->x, paddle->y - 1, 0);
		draw_pixel(paddle->x, paddle->y + PADDLE_HEIGHT - 1, 1);
	}
	else if (direction == -1)
	{
		draw_pixel(paddle->x, paddle->y, 1);
		draw_pixel(paddle->x, paddle->y + PADDLE_HEIGHT, 0);
	}
}

void draw_score(Score *score)
{
	if (score->num < 10)
	{
		draw_char(score->x, score->y, (char)(score->num + 48));
	}
	else
	{
		draw_string(score->x + 6, score->y, "10");
	}
}

void draw_dotted_line()
{
	draw_line(DISPLAY_SIZE_X / 2, 0, DISPLAY_SIZE_X / 2, DISPLAY_SIZE_Y - 1, dotted_line_func);
}

void init_ball(Ball *ball)
{
	ball->x = (DISPLAY_SIZE_X / 2) << 16;
	ball->y = (_random_number % DISPLAY_SIZE_Y) << 16;
	if (_random_number & 1)
		ball->dx = angleMap[_random_number % 8][0];
	else
		ball->dx = -angleMap[_random_number % 8][0];
	ball->dy = angleMap[_random_number % 8][1];
	ball->v = 1 << 16;
	draw_ball(ball);
}

void init_paddle(Paddle *paddle, int x)
{
	paddle->x = x;
	paddle->y = DISPLAY_SIZE_Y / 2 - PADDLE_HEIGHT / 2;
	draw_paddle(paddle);
}

void init_score(Score *score, int x, int y)
{
	score->num = 0;
	score->x = x;
	score->y = y;
	draw_score(score);
}

void update_ball(Ball *ball, Paddle *left_paddle, Paddle *right_paddle, Score *left_score, Score *right_score)
{
	erase_ball(ball);

	ball->x += fixed_point_multiply(ball->dx, ball->v);
	ball->y += fixed_point_multiply(ball->dy, ball->v);

	if (get_high_bits(ball->y) < 0)
	{
		ball->dy = -ball->dy;
		ball->y = 0 - ball->y + (1 << 16);
	}
	else if (get_high_bits(ball->y) >= DISPLAY_SIZE_Y)
	{
		ball->dy = -ball->dy;
		ball->y = (DISPLAY_SIZE_Y << 17) - ball->y - (1 << 16);
	}

	if (get_high_bits(ball->x) >= left_paddle->x && get_high_bits(ball->y) >= left_paddle->y && get_high_bits(ball->y) < left_paddle->y + PADDLE_HEIGHT)
	{
		int hit_area = (int)get_high_bits(ball->y) - left_paddle->y;
		ball->x = (2 * left_paddle->x - (int)get_high_bits(ball->x) - 1) << 16;

		ball->dx = -angleMap[hit_area][0];
		ball->dy = angleMap[hit_area][1];

		ball->v = fixed_point_multiply(ball->v, VELOCITY_FACTOR);
	}
	else if (get_high_bits(ball->x) <= right_paddle->x && get_high_bits(ball->y) >= right_paddle->y && get_high_bits(ball->y) < right_paddle->y + PADDLE_HEIGHT)
	{
		int hit_area = (int)get_high_bits(ball->y) - right_paddle->y;
		ball->x = (2 * right_paddle->x - (int)get_high_bits(ball->x) + 1) << 16;

		ball->dx = angleMap[hit_area][0];
		ball->dy = angleMap[hit_area][1];

		ball->v = fixed_point_multiply(ball->v, VELOCITY_FACTOR);
	}
	else if (get_high_bits(ball->x) < 0)
	{
		left_score->num++;
		draw_score(left_score);
		init_ball(ball);
		return;
	}
	else if (get_high_bits(ball->x) >= DISPLAY_SIZE_X)
	{
		right_score->num++;
		draw_score(right_score);
		init_ball(ball);
		return;
	}

	draw_ball(ball);
}

void move_paddle(Paddle *paddle, int direction)
{
	paddle->y += direction;
	if (paddle->y < 0)
	{
		paddle->y = 0;
		direction = 0;
	}
	else if (paddle->y > DISPLAY_SIZE_Y - PADDLE_HEIGHT)
	{
		paddle->y = DISPLAY_SIZE_Y - PADDLE_HEIGHT;
		direction = 0;
	}
	draw_paddle_increase(paddle, direction);
}

int player_input()
{
	unsigned char buffer = _motion_sensor_buffer;
	if (buffer & MOTION_SENSOR_U)
		return -1;
	else if (buffer & MOTION_SENSOR_D)
		return 1;
	else
		return 0;
}

int ai_input(Ball *ball, Paddle *ai_paddle, int *ai_rand)
{
	if (get_abs(ai_paddle->x - ball->x) <= ball->dx)
		*ai_rand = _random_number % PADDLE_HEIGHT;
	if ((int)get_high_bits(ball->y) < (ai_paddle->y + *ai_rand))
		return -1;
	else if ((int)get_high_bits(ball->y) > (ai_paddle->y + *ai_rand))
		return 1;
	else
		return 0;
}
int victory(Score *left_score, Score *right_score)
{
	if (left_score->num > MAX_SCORE)
	{
		display_clear();
		draw_string(81, 28, "Player Win!");
		display_refresh();
		return 1;
	}
	else if (right_score->num > MAX_SCORE)
	{
		display_clear();
		draw_string(87, 28, "Computer Win!");
		display_refresh();
		return 1;
	}

	return 0;
}

int main()
{
	Ball ball;
	Paddle left_paddle, right_paddle;
	Score left_score, right_score;

	int ai_rand = _random_number % PADDLE_HEIGHT;

	while (1)
	{
		display_clear();

		draw_dotted_line();
		init_paddle(&left_paddle, DISPLAY_SIZE_X - 3);
		init_paddle(&right_paddle, 2);
		init_score(&right_score, DISPLAY_SIZE_X / 4, 8);
		init_score(&left_score, 3 * DISPLAY_SIZE_X / 4, 8);
		init_ball(&ball);

		while (1)
		{
			move_paddle(&left_paddle, player_input());
			move_paddle(&right_paddle, ai_input(&ball, &right_paddle, &ai_rand));

			update_ball(&ball, &left_paddle, &right_paddle, &left_score, &right_score);

			if (victory(&left_score, &right_score))
				break;

			display_refresh();
		}

		driver_off();
	}
	return 0;
}
