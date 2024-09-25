#include "five_way_motion_sensor.h"
#include "display_96_64.h"
#include "random_number_generator.h"

#define PADDLE_HEIGHT 8
#define VELOCITY_FACTOR 1.2f
#define MAX_SCORE 10

const float angleMap[PADDLE_HEIGHT][2] = {{0.70710678f, 0.70710678f}, {0.8660254f, 0.5f}, {0.9659258f, 0.25881904f}, {1.0f, 0.0f}, {1.0f, 0.0f}, {0.9659258f, -0.25881904f}, {0.8660254f, -0.5f}, {0.70710678f, -0.70710678f}};

typedef struct
{
	float x, y;
	float dx, dy;
	float v;
} Ball;

typedef struct
{
	int x, y;
} Paddle;

void setpixel(int posX, int posY, unsigned char pixel)
{
	unsigned char *addr = &_display_buffer[posX / 8 + posY * 16];
	int offsetX = posX % 8;
	*addr &= ~(1 << offsetX);
	*addr ^= pixel << offsetX;
}

void draw_ball(Ball *ball)
{
	setpixel((int)ball->x, (int)ball->y, 1);
}

void draw_paddle(Paddle *paddle)
{
	for (int i = 0; i < PADDLE_HEIGHT; i++)
	{
		setpixel(paddle->x, paddle->y + i, 1);
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
	ball->x = DISPLAY_SIZE_X / 2;
	ball->y = _random_number % DISPLAY_SIZE_Y;
	ball->dx = angleMap[_random_number % 8][0];
	ball->dy = angleMap[_random_number % 8][1];
	ball->v = 1;
}

void init_paddle(Paddle *paddle, int x)
{
	paddle->x = x;
	paddle->y = DISPLAY_SIZE_Y / 2 - PADDLE_HEIGHT / 2;
}

void update_ball(Ball *ball, Paddle *left_paddle, Paddle *right_paddle, int *left_score, int *right_score)
{
	ball->x += ball->dx * ball->v;
	ball->y += ball->dy * ball->v;

	if (ball->y <= 0)
		ball->dy = 0 - ball->y;
	else if (ball->y >= DISPLAY_SIZE_Y - 1)
		ball->dy = 2 * (DISPLAY_SIZE_Y - 1) - ball->y;

	if (ball->x >= left_paddle->x && ball->y >= left_paddle->y && ball->y < left_paddle->y + PADDLE_HEIGHT)
	{
		int hit = (int)(ball->y - left_paddle->y);
		ball->x = 2 * left_paddle->x - ball->x;

		ball->dx = -angleMap[hit][0];
		ball->dy = angleMap[hit][1];

		ball->v *= VELOCITY_FACTOR;
	}
	else if (ball->x <= right_paddle->x && ball->y >= right_paddle->y && ball->y < right_paddle->y + PADDLE_HEIGHT)
	{
		int hit = (int)(ball->y - right_paddle->y);
		ball->x = 2 * right_paddle->x - ball->x;

		ball->dx = angleMap[hit][0];
		ball->dy = angleMap[hit][1];

		ball->v *= VELOCITY_FACTOR;
	}
	else if (ball->x <= 0)
	{
		(*right_score)++;
		init_ball(ball);
	}
	else if (ball->x >= DISPLAY_SIZE_X - 1)
	{
		(*left_score)++;
		init_ball(ball);
	}

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

int ai_input(Ball *ball, Paddle *ai_paddle)
{
	if (ball->y < (ai_paddle->y + PADDLE_HEIGHT / 2))
		return -1;
	else if (ball->y > (ai_paddle->y + PADDLE_HEIGHT / 2))
		return 1;
	else
		return 0;
}

int main()
{
	Ball ball;
	Paddle left_paddle, right_paddle;
	int left_score = 0, right_score = 0;

	init_ball(&ball);
	init_paddle(&left_paddle, DISPLAY_SIZE_X - 2);
	init_paddle(&right_paddle, 1);

	while (1)
	{
		display_clear();

		move_paddle(&left_paddle, player_input());
		move_paddle(&right_paddle, ai_input(&ball, &right_paddle));

		update_ball(&ball, &left_paddle, &right_paddle, &left_score, &right_score);

		if (left_score > MAX_SCORE)
		{
		}
		else if (right_score > MAX_SCORE)
		{
		}
		else
		{
		}

		display_refresh();
	}

	return 0;
}
