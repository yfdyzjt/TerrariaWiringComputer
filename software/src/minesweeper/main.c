#include "display_96_64.h"
#include "five_way_motion_sensor.h"
#include "random_number_generator.h"
#include "timer.h"

#include "minesweeper_grid.h"

#define BUTTON_X (GRID_WIDTH + 1)

#define BUTTON_TIME_Y 0
#define BUTTON_RESET_Y (2 * (GRID_HEIGHT + 1))
#define BUTTON_FLAG_Y (DISPLAY_SIZE_Y - 3 * (GRID_HEIGHT + 1))
#define BUTTON_COUNT_Y (DISPLAY_SIZE_Y - (GRID_HEIGHT + 1))

#define BOARD_WIDTH ((DISPLAY_SIZE_X - BUTTON_X) / (GRID_WIDTH + 1))
#define BOARD_HEIGHT (DISPLAY_SIZE_Y / (GRID_HEIGHT + 1))
#define BOARD_LENGTH (BOARD_WIDTH * BOARD_HEIGHT)
#define QUEUE_LENGTH ((3 * (BOARD_WIDTH + BOARD_HEIGHT - 2)) / 2)

enum Board_Type
{
    BOARD_MINE = 0b0001,
    BOARD_OPEN = 0b0010,
    BOARD_FLAG = 0b0100,
    BOARD_QUEUE = 0b1000,
};

enum State_Type
{
    STATE_END = 0b00001,
    STATE_FLAG = 0b00010,
    STATE_COUNT = 0b00100,
    STATE_FIRST = 0b01000,
    STATE_TIMER = 0b10000,
};

typedef struct
{
    signed char x, y;
} Point;

typedef struct
{
    Point data[QUEUE_LENGTH];
    short front;
    short rear;
} Queue;

typedef struct
{
    signed char x, y;

} Cursor;

Queue queue;
Cursor cursor;

signed char board[BOARD_WIDTH][BOARD_HEIGHT];

short mine_count = BOARD_LENGTH / 8;
short open_count;
short display_count;
short display_time;
unsigned char state;

char str_buffer[3];

char *itoa(int value, char *str, int base)
{
    char *ret = str;
    char negative = 0;
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
            negative = 1;
        }

        while (value != 0)
        {
            sum = value % base;
            *str++ = (sum > 9) ? (char)(sum - 10) + 'a' : (char)sum + '0';
            value /= base;
        }

        if (negative)
            *str++ = '-';

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

void swap(signed char *a, signed char *b)
{
    signed char temp = *a;
    *a = *b;
    *b = temp;
}

signed char max(signed char a, signed char b)
{
    return a > b ? a : b;
}

signed char min(signed char a, signed char b)
{
    return a < b ? a : b;
}

void queue_init()
{
    queue.front = 0;
    queue.rear = 0;
}

signed char queue_isempty()
{
    return queue.front == queue.rear;
}

void queue_push(signed char x, signed char y)
{
    queue.data[queue.rear].x = x;
    queue.data[queue.rear].y = y;
    queue.rear = (queue.rear + 1) % QUEUE_LENGTH;
}

void queue_pop(signed char *x, signed char *y)
{
    *x = queue.data[queue.front].x;
    *y = queue.data[queue.front].y;
    queue.front = (queue.front + 1) % QUEUE_LENGTH;
}

void button_grid_draw(signed char x, signed char y, signed char value)
{
    draw_grid_char(x,
                   y,
                   GRID_WIDTH,
                   GRID_HEIGHT,
                   (unsigned char *)grid[value]);
}

void board_grid_draw(signed char x, signed char y, signed char value)
{
    draw_grid_char(x * (GRID_WIDTH + 1) + BUTTON_X + 1,
                   y * (GRID_HEIGHT + 1) + 1,
                   GRID_WIDTH,
                   GRID_HEIGHT,
                   (unsigned char *)grid[value]);
}

void number_grid_draw(signed char x, signed char y, char *str)
{
    if (str[1] == '\0')
    {
        str[1] = str[0];
        str[0] = '0';
    }
    if (str[0] == '-')
        draw_grid_char(x - 3, y, 3, 5, (unsigned char *)number_grid[10]);
    else
        draw_grid_char(x - 3, y, 3, 5, (unsigned char *)number_grid[str[0] - '0']);
    draw_grid_char(x - 7, y, 3, 5, (unsigned char *)number_grid[str[1] - '0']);
}

void reset_draw(signed char value)
{
    button_grid_draw(1, BUTTON_RESET_Y + 1, value);
}

void flag_draw()
{
    button_grid_draw(1, BUTTON_FLAG_Y + 1, (state & STATE_FLAG) ? GRID_FLAG_ON : GRID_FLAG_OFF);
}

void mine_count_draw()
{
    signed char value = mine_count % 100;
    itoa(value, str_buffer, 10);
    number_grid_draw(BUTTON_X, BUTTON_COUNT_Y + 2, str_buffer);
}

void display_count_draw()
{
    signed char value = display_count % 100;
    itoa(value, str_buffer, 10);
    number_grid_draw(BUTTON_X, BUTTON_COUNT_Y + 2, str_buffer);
}

void display_time_draw()
{
    signed char value = display_time % 100;
    itoa(value, str_buffer, 10);
    number_grid_draw(BUTTON_X, BUTTON_TIME_Y + 2, str_buffer);
}

void cursor_draw(signed char value)
{
    draw_pixel(cursor.x + 0, cursor.y + 0, value);
    draw_pixel(cursor.x + 0, cursor.y + 2, value);
    draw_pixel(cursor.x + 0, cursor.y + 4, value);
    draw_pixel(cursor.x + 0, cursor.y + 6, value);

    draw_pixel(cursor.x + 2, cursor.y + 0, value);
    draw_pixel(cursor.x + 4, cursor.y + 0, value);
    draw_pixel(cursor.x + 6, cursor.y + 0, value);
    draw_pixel(cursor.x + 8, cursor.y + 0, value);

    draw_pixel(cursor.x + 8, cursor.y + 2, value);
    draw_pixel(cursor.x + 8, cursor.y + 4, value);
    draw_pixel(cursor.x + 8, cursor.y + 6, value);
    draw_pixel(cursor.x + 8, cursor.y + 8, value);

    draw_pixel(cursor.x + 0, cursor.y + 8, value);
    draw_pixel(cursor.x + 2, cursor.y + 8, value);
    draw_pixel(cursor.x + 4, cursor.y + 8, value);
    draw_pixel(cursor.x + 6, cursor.y + 8, value);
}

void time_start()
{
    _timer = display_time * 60;
    state |= STATE_TIMER;
    display_time_draw();
}

void time_stop()
{
    state &= ~STATE_TIMER;
    display_time_draw();
}

void time_init()
{
    display_time = 0;
    time_stop();
}

void time_update()
{
    if ((state & STATE_TIMER) && display_time != _timer / 60)
    {
        display_time = _timer / 60;
        display_time_draw();
    }
}

extern void display_all_mine();

void board_init()
{
    unsigned short i = 0, j;
    for (signed char x = 0; x < BOARD_WIDTH; x++)
        for (signed char y = 0; y < BOARD_HEIGHT; y++, i++)
        {
            if (i < BOARD_LENGTH - 1 - mine_count || i == BOARD_LENGTH - 1)
                board[x][y] = 0;
            else
            {
                board[x][y] = BOARD_MINE;
                j = rand.u32 % (i + 1);
                swap(&board[x][y], &board[j % BOARD_WIDTH][j / BOARD_WIDTH]);
            }
            board_grid_draw(x, y, GRID_NOPEN);
        }
}

void game_init()
{
    state &= ~STATE_END;
    state |= STATE_FIRST;

    open_count = 0;
    display_count = mine_count;

    display_count_draw();
    flag_draw();
    reset_draw(GRID_RESET_NORM);

    time_init();
    board_init();
}

void flag_board(signed char x, signed char y)
{
    board[x][y] ^= BOARD_FLAG;
    if (board[x][y] & BOARD_FLAG)
    {
        board_grid_draw(x, y, GRID_FLAG);
        display_count--;
        display_count_draw();
    }
    else
    {
        board_grid_draw(x, y, GRID_NOPEN);
        display_count++;
        display_count_draw();
    }
}

char adjacent_count(signed char x, signed char y, enum Board_Type value)
{
    signed char count = 0;
    signed char i_min = max(x - 1, 0), i_max = min(x + 1, BOARD_WIDTH - 1);
    signed char j_min = max(y - 1, 0), j_max = min(y + 1, BOARD_HEIGHT - 1);
    for (signed char i = i_min; i <= i_max; i++)
        for (signed char j = j_min; j <= j_max; j++)
            if (board[i][j] & value)
                count++;
    return count;
}

void display_all_mine()
{
    for (signed char i = 0; i < BOARD_WIDTH; i++)
        for (signed char j = 0; j < BOARD_HEIGHT; j++)
            if (board[i][j] & BOARD_MINE)
                board_grid_draw(i, j, GRID_MINE);
    display_refresh();
}

signed char end_win()
{
    reset_draw(GRID_RESET_WIN);
    display_refresh();
    time_stop();
    state |= STATE_END;
    return 1;
}

signed char end_fail()
{
    reset_draw(GRID_RESET_FAIL);
    display_refresh();
    display_all_mine();
    time_stop();
    state |= STATE_END;
    return 1;
}

signed char click_board(signed char x, signed char y)
{
    signed char cur_x, cur_y;
    signed char adjacent_mine_count;

    if (state & STATE_FIRST)
    {
        time_start();
        state &= ~STATE_FIRST;
        if (board[x][y] & BOARD_MINE)
            swap(&board[x][y], &board[BOARD_WIDTH - 1][BOARD_HEIGHT - 1]);
    }

    if (board[x][y] & BOARD_MINE)
        return end_fail();

    queue_init();
    queue_push(x, y);
    while (!queue_isempty())
    {
        queue_pop(&cur_x, &cur_y);

        if (board[cur_x][cur_y] & (BOARD_OPEN | BOARD_MINE | BOARD_FLAG))
            continue;

        adjacent_mine_count = adjacent_count(cur_x, cur_y, BOARD_MINE);

        board[cur_x][cur_y] |= BOARD_OPEN;
        board[cur_x][cur_y] |= (adjacent_mine_count & 0b1111) << 4;

        board_grid_draw(cur_x, cur_y, adjacent_mine_count);

        if (adjacent_mine_count == 0)
        {
            signed char i_min = max(cur_x - 1, 0), i_max = min(cur_x + 1, BOARD_WIDTH - 1);
            signed char j_min = max(cur_y - 1, 0), j_max = min(cur_y + 1, BOARD_HEIGHT - 1);
            for (signed char i = i_min; i <= i_max; i++)
                for (signed char j = j_min; j <= j_max; j++)
                    if (!(board[i][j] & (BOARD_OPEN | BOARD_MINE | BOARD_FLAG | BOARD_QUEUE)))
                    {
                        board[i][j] |= BOARD_QUEUE;
                        queue_push(i, j);
                    }
        }

        open_count++;
        if (open_count >= BOARD_LENGTH - mine_count)
            return end_win();
    }
    return 0;
}

void double_click_board(signed char x, signed char y)
{
    signed char adjacent_flag_count = adjacent_count(x, y, BOARD_FLAG);
    signed char adjacent_mine_count = (board[x][y] >> 4) & 0b1111;

    if (adjacent_flag_count == adjacent_mine_count && adjacent_mine_count != 0)
    {
        signed char i_min = max(x - 1, 0), i_max = min(x + 1, BOARD_WIDTH - 1);
        signed char j_min = max(y - 1, 0), j_max = min(y + 1, BOARD_HEIGHT - 1);
        for (signed char i = i_min; i <= i_max; i++)
            for (signed char j = j_min; j <= j_max; j++)
                if (!(board[i][j] & (BOARD_OPEN | BOARD_FLAG)))
                    if (click_board(i, j))
                        return;
    }
}

void cursor_u()
{
    if (cursor.y != 0)
        cursor.y -= GRID_HEIGHT + 1;
}

void cursor_d()
{
    if (cursor.y != (GRID_HEIGHT + 1) * (BOARD_HEIGHT - 1))
        cursor.y += GRID_HEIGHT + 1;
}

void cursor_r()
{
    if (cursor.x != 0)
        cursor.x -= GRID_WIDTH + 1;
}

void cursor_l()
{
    if (cursor.x != (GRID_WIDTH + 1) * (BOARD_WIDTH - 1) + BUTTON_X)
        cursor.x += GRID_WIDTH + 1;
}

void cursor_j()
{
    if (cursor.x == 0)
    {
        switch (cursor.y)
        {
        case BUTTON_RESET_Y:
            game_init();
            break;
        case BUTTON_FLAG_Y:
            state ^= STATE_FLAG;
            flag_draw();
            break;
        case BUTTON_COUNT_Y:
            state ^= STATE_COUNT;
            mine_count_draw();
            break;
        }
    }
    else
    {
        if (!(state & STATE_END))
        {
            signed char x = ((cursor.x - BUTTON_X) / (GRID_WIDTH + 1));
            signed char y = (cursor.y / (GRID_HEIGHT + 1));

            if (state & STATE_FLAG)
            {
                if (board[x][y] & BOARD_OPEN)
                    double_click_board(x, y);
                else if (!(state & STATE_FIRST))
                    flag_board(x, y);
            }
            else
            {
                if (!(board[x][y] & BOARD_OPEN))
                    click_board(x, y);
                else
                    double_click_board(x, y);
            }
        }
    }
}

void cursor_count_u()
{
    if (mine_count != BOARD_LENGTH - 1)
    {
        mine_count++;
        mine_count_draw();
    }
}

void cursor_count_d()
{
    if (mine_count != 0)
    {
        mine_count--;
        mine_count_draw();
    }
}

void cursor_count_j()
{
    state ^= STATE_COUNT;
    game_init();
}

void cursor_update()
{
    cursor_draw(0);
    motion_sensor_refresh();
    if (state & STATE_COUNT)
    {
        if (motion_sensor_u())
            cursor_count_u();
        if (motion_sensor_d())
            cursor_count_d();
        if (motion_sensor_j())
            cursor_count_j();
    }
    else
    {
        if (motion_sensor_u())
            cursor_u();
        if (motion_sensor_d())
            cursor_d();
        if (motion_sensor_r())
            cursor_r();
        if (motion_sensor_l())
            cursor_l();
        if (motion_sensor_j())
            cursor_j();
    }
    cursor_draw(1);
}

int main()
{
    game_init();
    while (1)
    {
        cursor_update();
        time_update();
        display_refresh();
    }
    return 0;
}