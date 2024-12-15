#include "helper.h"

#include "display_96_64.h"

#define NUM_GRID_H 5
#define NUM_GRID_W 3

const unsigned char number_grid[10][NUM_GRID_H] = {
    {0b111, 0b101, 0b101, 0b101, 0b111},
    {0b010, 0b110, 0b010, 0b010, 0b111},
    {0b110, 0b001, 0b010, 0b100, 0b111},
    {0b110, 0b001, 0b010, 0b001, 0b110},
    {0b101, 0b101, 0b111, 0b001, 0b001},
    {0b111, 0b100, 0b110, 0b001, 0b110},
    {0b011, 0b100, 0b111, 0b101, 0b111},
    {0b111, 0b001, 0b010, 0b010, 0b010},
    {0b111, 0b101, 0b111, 0b101, 0b111},
    {0b111, 0b101, 0b111, 0b001, 0b110},
};

int x = DISPLAY_SIZE_X - NUM_GRID_W, y = 0;
char str[10];

signed char itoa_l(int value, char *str, int base)
{
    char *ret = str;
    signed char negative = 0;

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

    return (signed char)(str - ret);
}

void output(char c)
{
    if (c >= '0' && c <= '9')
    {
        draw_grid_char(x, y, 3, 5, number_grid[c - '0']);
        display_refresh();
        x -= NUM_GRID_W + 1;
    }

    if (x < 0 || c == '\n')
    {
        y += NUM_GRID_H + 1;
        x = DISPLAY_SIZE_X - NUM_GRID_W;
    }
    if (y >= DISPLAY_SIZE_Y)
    {
        y = 0;
    }
}

void output_num_4(int num)
{
    signed char l = itoa_l(num, str, 10);
    for (int i = 0; i < 4 - l; i++)
        output('0');
    for (int i = 0; i < l; i++)
        output(str[i]);
}