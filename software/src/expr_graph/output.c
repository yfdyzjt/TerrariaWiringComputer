#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "output.h"
#include "display_96_64.h"
#include "tinyexpr.h"

#define GRAPH_POS_X 0
#define GRAPH_POS_Y 9
#define GRAPH_SIZE_X DISPLAY_SIZE_X
#define GRAPH_SIZE_Y (DISPLAY_SIZE_Y - GRAPH_POS_Y + 1)

#define SCREEN_TO_REAL_X(px) (double)(((GRAPH_POS_X + GRAPH_SIZE_X / 2 - px) / params.scale_x) + params.center_x)
#define SCREEN_TO_REAL_Y(py) (double)(((GRAPH_POS_Y + GRAPH_SIZE_Y / 2 - py) / params.scale_y) + params.center_y)
#define REAL_TO_SCREEN_X(x) (int32_t)round(GRAPH_POS_X + GRAPH_SIZE_X / 2 - ((x - params.center_x) * params.scale_x))
#define REAL_TO_SCREEN_Y(y) (int32_t)round(GRAPH_POS_Y + GRAPH_SIZE_Y / 2 - ((y - params.center_y) * params.scale_y))

expr_graph_params params;

void draw_init(const char *input)
{
    display_clear();
    draw_line(GRAPH_POS_X, GRAPH_POS_Y - 1,
              GRAPH_POS_X + GRAPH_SIZE_X - 1, GRAPH_POS_Y - 1,
              solid_line_one_func);
    draw_string(GRAPH_SIZE_X + 1, 0, "y=");
    draw_string(GRAPH_SIZE_X - 11, 0, input);
    display_refresh();
}

static int32_t calculate_x_intersection(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t y_boundary)
{
    if (y0 == y1)
        return x0;
    return x0 + ((x1 - x0) * (y_boundary - y0)) / (y1 - y0);
}

static int32_t calculate_y_intersection(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x_boundary)
{
    if (x0 == x1)
        return y0;
    return y0 + ((y1 - y0) * (x_boundary - x0)) / (x1 - x0);
}

#define INSIDE 0
#define LEFT 1
#define RIGHT 2
#define BOTTOM 4
#define TOP 8

static int32_t compute_code(int32_t x, int32_t y)
{
    int32_t code = INSIDE;
    if (x < GRAPH_POS_X)
        code |= LEFT;
    else if (x >= GRAPH_POS_X + GRAPH_SIZE_X)
        code |= RIGHT;
    if (y < GRAPH_POS_Y)
        code |= BOTTOM;
    else if (y >= GRAPH_POS_Y + GRAPH_SIZE_Y)
        code |= TOP;
    return code;
}

static bool clip_line(int32_t *x0, int32_t *y0, int32_t *x1, int32_t *y1)
{
    int32_t code0 = compute_code(*x0, *y0);
    int32_t code1 = compute_code(*x1, *y1);
    bool accept = false;

    while (true)
    {
        if (!(code0 | code1))
        {
            accept = true;
            break;
        }
        else if (code0 & code1)
        {
            break;
        }
        else
        {
            int32_t code_out = code0 ? code0 : code1;
            int32_t x = *x0, y = *y0;

            if (code_out & TOP)
            {
                x = calculate_x_intersection(*x0, *y0, *x1, *y1, GRAPH_POS_Y + GRAPH_SIZE_Y - 1);
                y = GRAPH_POS_Y + GRAPH_SIZE_Y - 1;
            }
            else if (code_out & BOTTOM)
            {
                x = calculate_x_intersection(*x0, *y0, *x1, *y1, GRAPH_POS_Y);
                y = GRAPH_POS_Y;
            }
            else if (code_out & RIGHT)
            {
                y = calculate_y_intersection(*x0, *y0, *x1, *y1, GRAPH_POS_X + GRAPH_SIZE_X - 1);
                x = GRAPH_POS_X + GRAPH_SIZE_X - 1;
            }
            else if (code_out & LEFT)
            {
                y = calculate_y_intersection(*x0, *y0, *x1, *y1, GRAPH_POS_X);
                x = GRAPH_POS_X;
            }

            if (code_out == code0)
            {
                *x0 = x;
                *y0 = y;
                code0 = compute_code(x, y);
            }
            else
            {
                *x1 = x;
                *y1 = y;
                code1 = compute_code(x, y);
            }
        }
    }
    return accept;
}
void draw_expr(te_expr *expr, double *x)
{
    int32_t center_screen_x = REAL_TO_SCREEN_X(0);
    int32_t center_screen_y = REAL_TO_SCREEN_Y(0);
    if (center_screen_y >= GRAPH_POS_Y && center_screen_y < GRAPH_POS_Y + GRAPH_SIZE_Y)
        draw_line(GRAPH_POS_X, center_screen_y,
                  GRAPH_POS_X + GRAPH_SIZE_X, center_screen_y,
                  solid_line_one_func);
    if (center_screen_x >= GRAPH_POS_X && center_screen_x < GRAPH_POS_X + GRAPH_SIZE_X)
        draw_line(center_screen_x, GRAPH_POS_Y,
                  center_screen_x, GRAPH_POS_Y + GRAPH_SIZE_Y,
                  solid_line_one_func);
    display_refresh();

    int32_t last_pixel_x = GRAPH_POS_X - 1;
    *x = SCREEN_TO_REAL_X(last_pixel_x);
    double last_y = te_eval(expr);
    int32_t last_pixel_y = REAL_TO_SCREEN_Y(last_y);

    for (int32_t pixel_x = GRAPH_POS_X; pixel_x <= GRAPH_POS_X + GRAPH_SIZE_X; pixel_x++)
    {
        *x = SCREEN_TO_REAL_X(pixel_x);
        double current_y = te_eval(expr);
        int32_t current_pixel_y = REAL_TO_SCREEN_Y(current_y);

        int32_t x0 = last_pixel_x, y0 = last_pixel_y;
        int32_t x1 = pixel_x, y1 = current_pixel_y;

        if (clip_line(&x0, &y0, &x1, &y1))
        {
            if (abs(y1 - y0) <= 1)
                draw_pixel(x1, y1, 1);
            else
                draw_line(x0, y0, x1, y1, solid_line_one_func);
        }

        last_pixel_x = pixel_x;
        last_pixel_y = current_pixel_y;

        display_refresh();
    }
}
