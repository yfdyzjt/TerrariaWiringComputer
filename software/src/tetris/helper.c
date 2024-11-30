#include "helper.h"

const unsigned char Mod37BitPosition[] = {32, 0, 1, 26, 2, 23, 27, 0, 3, 16, 24, 30, 28, 11, 0, 13, 4, 7, 17, 0, 25, 22, 31, 15, 29, 10, 12, 6, 0, 21, 14, 9, 5, 20, 8, 19, 18};

unsigned char zero_count(int value)
{
    return Mod37BitPosition[(-value & value) % 37];
}

int max(int a, int b)
{
    return a > b ? a : b;
}

int min(int a, int b)
{
    return a < b ? a : b;
}

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
