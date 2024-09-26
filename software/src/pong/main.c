#include "five_way_motion_sensor.h"
#include "display_96_64.h"
#include "random_number_generator.h"

#include "ascii_5_7.h"

#define PADDLE_HEIGHT 8
#define VELOCITY_FACTOR 78643
#define MAX_SCORE 10

const int angleMap[PADDLE_HEIGHT][2] = {{46341, -46341}, {56756, -32768}, {63303, -16962}, {65536, 0}, {65536, 0}, {63303, 16962}, {56756, 32768}, {46341, 46341}};

typedef struct
{
	int x, y;
	int dx, dy;
	int v;
} Ball;

typedef struct
{
	int x, y;
} Paddle;

void draw_grid(int posX, int posY, int sizeX, int sizeY, unsigned char *grid)
{
	unsigned char *addr = &_display_buffer[posX / 8 + posY * 16];

	int i;

	int high, low;
	unsigned short mask, date;

	low = posX % 8;
	high = low + sizeX;
	mask = (unsigned short)(((1 << high) - 1) ^ ((1 << low) - 1));

	for (i = 0; i < sizeY; i++)
	{
		if (i + posY < 0 && i + posY >= DISPLAY_SIZE_Y)
			continue;
		date = (unsigned short)((grid[i] << low) & mask);

		if (posX >= 0 && posX < DISPLAY_SIZE_X)
			*addr = (*addr & ~*((unsigned char *)&mask)) ^ *((unsigned char *)&date);
		if (high >= 8 && posX + 8 >= 0 && posX + 8 < DISPLAY_SIZE_X)
			*(addr + 1) = (*(addr + 1) & ~*((unsigned char *)&mask + 1)) ^ *((unsigned char *)&date + 1);
		addr += 16;
	}
}

void draw_pixel(int posX, int posY, unsigned char pixel)
{
	unsigned char *addr = &_display_buffer[posX / 8 + posY * 16];
	int offsetX = posX % 8;
	*addr &= ~(1 << offsetX);
	*addr ^= pixel << offsetX;
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
	draw_pixel((int)get_high_bits(ball->x), (int)get_high_bits(ball->y), 1);
}

void draw_paddle(Paddle *paddle)
{
	for (int i = 0; i < PADDLE_HEIGHT; i++)
	{
		draw_pixel(paddle->x, paddle->y + i, 1);
	}
}

void draw_score(int score, int x, int y)
{
	switch (score)
	{
	case 0:
		draw_grid(x, y, 5, 7, (unsigned char *)g57Ascii['0' - 32]);
		break;
	case 1:
		draw_grid(x, y, 5, 7, (unsigned char *)g57Ascii['1' - 32]);
		break;
	case 2:
		draw_grid(x, y, 5, 7, (unsigned char *)g57Ascii['2' - 32]);
		break;
	case 3:
		draw_grid(x, y, 5, 7, (unsigned char *)g57Ascii['3' - 32]);
		break;
	case 4:
		draw_grid(x, y, 5, 7, (unsigned char *)g57Ascii['4' - 32]);
		break;
	case 5:
		draw_grid(x, y, 5, 7, (unsigned char *)g57Ascii['5' - 32]);
		break;
	case 6:
		draw_grid(x, y, 5, 7, (unsigned char *)g57Ascii['6' - 32]);
		break;
	case 7:
		draw_grid(x, y, 5, 7, (unsigned char *)g57Ascii['7' - 32]);
		break;
	case 8:
		draw_grid(x, y, 5, 7, (unsigned char *)g57Ascii['8' - 32]);
		break;
	case 9:
		draw_grid(x, y, 5, 7, (unsigned char *)g57Ascii['9' - 32]);
		break;
	case 10:
		draw_grid(x, y, 5, 7, (unsigned char *)g57Ascii['1' - 32]);
		draw_grid(x + 6, y, 5, 7, (unsigned char *)g57Ascii['0' - 32]);
		break;
	default:
		break;
	}
}

void draw_line(int x)
{
	for (int i = 0; i < DISPLAY_SIZE_Y; i += 2)
	{
		draw_pixel(x, i, 1);
	}
}

void draw_string(char *s, int x, int y)
{
	int i = 0;
	while (s[i] != '\0')
	{
		draw_grid(x, y, 5, 7, (unsigned char *)g57Ascii[s[i++] - 32]);
		x -= 6;
	}
}

void display_clear()
{
	*_display_ctrl = DISPLAY_CLC;
}

void display_refresh()
{
	*_display_ctrl = DISPLAY_REF;
}

void init_ball(Ball *ball)
{
	ball->x = (DISPLAY_SIZE_X / 2) << 16;
	ball->y = (_random_number % DISPLAY_SIZE_Y) << 16;
	ball->dx = angleMap[_random_number % 8][0];
	ball->dy = angleMap[_random_number % 8][1];
	ball->v = 1 << 16;
}

void init_paddle(Paddle *paddle, int x)
{
	paddle->x = x;
	paddle->y = DISPLAY_SIZE_Y / 2 - PADDLE_HEIGHT / 2;
}

void update_ball(Ball *ball, Paddle *left_paddle, Paddle *right_paddle, int *left_score, int *right_score)
{
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
		(*right_score)++;
		init_ball(ball);
	}
	else if (get_high_bits(ball->x) >= DISPLAY_SIZE_X)
	{
		(*left_score)++;
		init_ball(ball);
	}

	draw_score(*left_score, DISPLAY_SIZE_X / 4, 8);
	draw_score(*right_score, 3 * DISPLAY_SIZE_X / 4, 8);
	draw_line(DISPLAY_SIZE_X / 2);
	draw_ball(ball);
}

void move_paddle(Paddle *paddle, int direction)
{
	paddle->y += direction;
	if (paddle->y < 0)
		paddle->y = 0;
	else if (paddle->y > DISPLAY_SIZE_Y - PADDLE_HEIGHT)
		paddle->y = DISPLAY_SIZE_Y - PADDLE_HEIGHT;
	draw_paddle(paddle);
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
	if (get_abs(ball->x + ball->dx) <= ball->dx)
		*ai_rand = _random_number % PADDLE_HEIGHT;
	if ((int)get_high_bits(ball->y) < (ai_paddle->y + *ai_rand))
		return -1;
	else if ((int)get_high_bits(ball->y) > (ai_paddle->y + *ai_rand))
		return 1;
	else
		return 0;
}
int victory(int *left_score, int *right_score)
{
	if (*left_score > MAX_SCORE)
	{
		display_clear();
		draw_string("Player Win!", 15, 24);
		return 1;
	}
	else if (*right_score > MAX_SCORE)
	{
		display_clear();
		draw_string("Computer Win!", 9, 24);
		return 1;
	}

	return 0;
}

int main()
{
	Ball ball;
	Paddle left_paddle, right_paddle;
	int left_score = 0, right_score = 0;
	int ai_rand = _random_number % PADDLE_HEIGHT;

	while (1)
	{
		init_ball(&ball);
		init_paddle(&left_paddle, DISPLAY_SIZE_X - 3);
		init_paddle(&right_paddle, 2);

		while (1)
		{
			display_clear();

			move_paddle(&left_paddle, player_input());
			move_paddle(&right_paddle, ai_input(&ball, &right_paddle, &ai_rand));

			update_ball(&ball, &left_paddle, &right_paddle, &left_score, &right_score);

			if (victory(&left_score, &right_score))
				break;

			display_refresh();
		}
		while (1)
		{
			if (_motion_sensor_buffer & MOTION_SENSOR_J)
				break;
		}
	}
	return 0;
}
