#include "helper.h"

#include "display_96_64.h"

int x = DISPLAY_SIZE_X - 6, y = 0;
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
    if (c != '\n')
    {
        draw_char(x, y, c);
        display_refresh();
        x -= 6;
    }

    if (x < 0 || c == '\n')
    {
        y += 8;
        x = DISPLAY_SIZE_X - 6;
    }
    if (y >= DISPLAY_SIZE_Y)
    {
        y = 0;
    }
}

void output_num(int num)
{
    signed char l = itoa_l(num, str, 10);
    for (int i = 0; i < l; i++)
        output(str[i]);
    output('\n');
}