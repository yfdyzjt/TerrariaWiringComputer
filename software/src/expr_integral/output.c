#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "output.h"
#include "display_96_64.h"
#include "tinyexpr.h"

#define CHARACTER_WIDTH 5
#define CHARACTER_HEIGHT 7

#define INTEGRAL_SYMBOL_POS_X DISPLAY_SIZE_X
#define INTEGRAL_SYMBOL_POS_Y 0

#define INTEGRAL_SYMBOL_SIZE_X 7
#define INTEGRAL_SYMBOL_SIZE_Y 15

const uint8_t integral_symbol_grid[INTEGRAL_SYMBOL_SIZE_Y] = {
    0b0000110,
    0b0001001,
    0b0001000,
    0b0001000,
    0b0001000,
    0b0001000,
    0b0001000,
    0b0001000,
    0b0001000,
    0b0001000,
    0b0001000,
    0b0001000,
    0b0001000,
    0b1001000,
    0b0110000,
};

static int32_t max(int32_t a, int32_t b)
{
    return (a > b) ? a : b;
}

void draw_init()
{
    display_clear();
    display_refresh();
}

void draw_integral_expr(const char *expr_input, const char *lower_input, const char *upper_input)
{
    int32_t bound_len = max(strlen(lower_input), strlen(upper_input));
    int32_t expr_len = strlen(expr_input);

    draw_grid_char(INTEGRAL_SYMBOL_POS_X - INTEGRAL_SYMBOL_SIZE_X,
                   INTEGRAL_SYMBOL_POS_Y,
                   INTEGRAL_SYMBOL_SIZE_X, INTEGRAL_SYMBOL_SIZE_Y,
                   integral_symbol_grid);
    draw_string(INTEGRAL_SYMBOL_POS_X - INTEGRAL_SYMBOL_SIZE_X,
                INTEGRAL_SYMBOL_POS_Y,
                upper_input);
    draw_string(INTEGRAL_SYMBOL_POS_X - INTEGRAL_SYMBOL_SIZE_X,
                INTEGRAL_SYMBOL_POS_Y + CHARACTER_HEIGHT + 1,
                lower_input);
    draw_string(INTEGRAL_SYMBOL_POS_X - INTEGRAL_SYMBOL_SIZE_X - bound_len * (CHARACTER_WIDTH + 1),
                INTEGRAL_SYMBOL_POS_Y + INTEGRAL_SYMBOL_SIZE_Y / 2 - CHARACTER_HEIGHT / 2,
                expr_input);
    draw_string(INTEGRAL_SYMBOL_POS_X - INTEGRAL_SYMBOL_SIZE_X - (bound_len + expr_len) * (CHARACTER_WIDTH + 1),
                INTEGRAL_SYMBOL_POS_Y + INTEGRAL_SYMBOL_SIZE_Y / 2 - CHARACTER_HEIGHT / 2,
                "dx");
    display_refresh();
}

void draw_integral_result(const double *result)
{
    char str[32];
    snprintf(str, sizeof(str), "%.10g", *result);
    draw_string(INTEGRAL_SYMBOL_POS_X + 1,
                INTEGRAL_SYMBOL_POS_Y + INTEGRAL_SYMBOL_SIZE_Y + 1, "=");
    draw_string(INTEGRAL_SYMBOL_POS_X - 5,
                INTEGRAL_SYMBOL_POS_Y + INTEGRAL_SYMBOL_SIZE_Y + 1, str);
    display_refresh();
}
