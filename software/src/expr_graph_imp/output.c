#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include "output.h"
#include "display_96_64.h"
#include "tinyexpr.h"

#define GRAPH_POS_X 0
#define GRAPH_POS_Y 9
#define GRAPH_SIZE_X DISPLAY_SIZE_X
#define GRAPH_SIZE_Y (DISPLAY_SIZE_Y - GRAPH_POS_Y + 1)

#define SCREEN_TO_REAL_X(px) (double)(((GRAPH_POS_X + GRAPH_SIZE_X / 2 - px) / params.scale_x) + params.center_x)
#define SCREEN_TO_REAL_Y(py) (double)(((GRAPH_POS_Y + GRAPH_SIZE_Y / 2 - py) / params.scale_y) + params.center_y)
#define REAL_TO_SCREEN_X(x) (int32_t)(GRAPH_POS_X + GRAPH_SIZE_X / 2 - ((x - params.center_x) * params.scale_x))
#define REAL_TO_SCREEN_Y(y) (int32_t)(GRAPH_POS_Y + GRAPH_SIZE_Y / 2 - ((y - params.center_y) * params.scale_y))

expr_graph_params params;

void draw_init(const char *input)
{
    display_clear();
    draw_line(GRAPH_POS_X, GRAPH_POS_Y - 1,
              GRAPH_POS_X + GRAPH_SIZE_X - 1, GRAPH_POS_Y - 1,
              solid_line_one_func);
    draw_string(GRAPH_SIZE_X + 1, 0, input);
    display_refresh();
}

#define MAX_DEPTH 6
#define MS_SIZE 2
#define EPSILON 1e-6

static const int8_t ms_points[16] = {
    0b0000, // 0000
    0b0001, // 0001
    0b0010, // 0010
    0b0011, // 0011
    0b0100, // 0100
    0b0101, // 0101
    0b1111, // 0110
    0b1000, // 0111
    0b1000, // 1000
    0b1111, // 1001
    0b0101, // 1010
    0b0100, // 1011
    0b0011, // 1100
    0b0010, // 1101
    0b0001, // 1110
    0b0000, // 1111
};

static void draw_marching_squares(int32_t x1, int32_t y1, int32_t x2, int32_t y2,
                                  double *f1, double *f2, double *f3, double *f4)
{
    uint32_t index = 0;
    if (*f1 > 0)
        index |= 1;
    if (*f2 > 0)
        index |= 2;
    if (*f3 > 0)
        index |= 4;
    if (*f4 > 0)
        index |= 8;

    if (index == 0 || index == 15)
        return;

    int8_t segments = ms_points[index];

    if (segments & 1)
        draw_pixel(x1, y1, 1);
    if (segments & 2)
        draw_pixel(x1 + 1, y1, 1);
    if (segments & 4)
        draw_pixel(x1, y1 + 1, 1);
    if (segments & 8)
        draw_pixel(x1 + 1, y1 + 1, 1);
    display_refresh();
}

static bool is_zero(double value)
{
    return fabs(value) < EPSILON;
}

static bool cell_may_contain_curve(te_expr *expr, double *x, double *y,
                                   int32_t x1, int32_t y1, int32_t x2, int32_t y2,
                                   double *f1, double *f2, double *f3, double *f4)
{
    double rx1 = SCREEN_TO_REAL_X(x1), rx2 = SCREEN_TO_REAL_X(x2);
    double ry1 = SCREEN_TO_REAL_Y(y1), ry2 = SCREEN_TO_REAL_Y(y2);

    if (isnan(*f1))
    {
        *x = rx1;
        *y = ry1;
        *f1 = te_eval(expr);
    }
    if (isnan(*f2))
    {
        *x = rx2;
        *y = ry1;
        *f2 = te_eval(expr);
    }
    if (isnan(*f3))
    {
        *x = rx1;
        *y = ry2;
        *f3 = te_eval(expr);
    }
    if (isnan(*f4))
    {
        *x = rx2;
        *y = ry2;
        *f4 = te_eval(expr);
    }

    if (is_zero(*f1) || is_zero(*f2) || is_zero(*f3) || is_zero(*f4))
        return true;

    if ((*f1 > 0) != (*f2 > 0) || (*f1 > 0) != (*f3 > 0) || (*f1 > 0) != (*f4 > 0))
        return true;

    return false;
}

static void draw_implicit_region(te_expr *expr, double *x, double *y,
                                 int32_t x1, int32_t y1, int32_t x2, int32_t y2,
                                 int32_t depth,
                                 double *f1, double *f2, double *f3, double *f4)
{
    if (x1 >= GRAPH_POS_X + GRAPH_SIZE_X || x2 < GRAPH_POS_X ||
        y1 >= GRAPH_POS_Y + GRAPH_SIZE_Y || y2 < GRAPH_POS_Y)
        return;

    x1 = x1 < GRAPH_POS_X ? GRAPH_POS_X : x1;
    y1 = y1 < GRAPH_POS_Y ? GRAPH_POS_Y : y1;
    x2 = x2 >= GRAPH_POS_X + GRAPH_SIZE_X ? GRAPH_POS_X + GRAPH_SIZE_X - 1 : x2;
    y2 = y2 >= GRAPH_POS_Y + GRAPH_SIZE_Y ? GRAPH_POS_Y + GRAPH_SIZE_Y - 1 : y2;

    if (depth >= MAX_DEPTH || (x2 - x1 <= MS_SIZE && y2 - y1 <= MS_SIZE))
    {
        if (cell_may_contain_curve(expr, x, y, x1, y1, x2, y2, f1, f2, f3, f4))
        {
            /*
            int32_t cx = (x1 + x2) / 2;
            int32_t cy = (y1 + y2) / 2;
            draw_pixel(cx, cy, 1);
            */
            draw_marching_squares(x1, y1, x2, y2, f1, f2, f3, f4);
        }
        return;
    }

    if (!cell_may_contain_curve(expr, x, y, x1, y1, x2, y2, f1, f2, f3, f4))
        return;

    int32_t xm = (x1 + x2) / 2;
    int32_t ym = (y1 + y2) / 2;

    double fmm = NAN, fm1 = NAN, fm2 = NAN, f1m = NAN, f2m = NAN;

    draw_implicit_region(expr, x, y, x1, y1, xm, ym, depth + 1, f1, &fm1, &f1m, &fmm);
    draw_implicit_region(expr, x, y, xm, y1, x2, ym, depth + 1, &fm1, f2, &fmm, &f2m);
    draw_implicit_region(expr, x, y, x1, ym, xm, y2, depth + 1, &f1m, &fmm, f3, &fm2);
    draw_implicit_region(expr, x, y, xm, ym, x2, y2, depth + 1, &fmm, &f2m, &fm2, f4);
}

void draw_implicit_expr(te_expr *expr, double *x, double *y)
{
    int32_t center_screen_x = REAL_TO_SCREEN_X(0);
    int32_t center_screen_y = REAL_TO_SCREEN_Y(0);
    if (center_screen_y >= GRAPH_POS_Y && center_screen_y < GRAPH_POS_Y + GRAPH_SIZE_Y)
        draw_line(GRAPH_POS_X, center_screen_y,
                  GRAPH_POS_X + GRAPH_SIZE_X - 1, center_screen_y,
                  solid_line_one_func);
    if (center_screen_x >= GRAPH_POS_X && center_screen_x < GRAPH_POS_X + GRAPH_SIZE_X)
        draw_line(center_screen_x, GRAPH_POS_Y,
                  center_screen_x, GRAPH_POS_Y + GRAPH_SIZE_Y - 1,
                  solid_line_one_func);
    display_refresh();

    int32_t x1 = GRAPH_POS_X, x2 = GRAPH_POS_X + GRAPH_SIZE_X - 1;
    int32_t y1 = GRAPH_POS_Y, y2 = GRAPH_POS_Y + GRAPH_SIZE_Y - 1;

    int32_t xm = (x1 + x2) / 2;
    int32_t ym = (y1 + y2) / 2;

    double fmm = NAN, fm1 = NAN, fm2 = NAN, f1m = NAN, f2m = NAN;
    double f1 = NAN, f2 = NAN, f3 = NAN, f4 = NAN;

    draw_implicit_region(expr, x, y, x1, y1, xm, ym, 0, &f1, &fm1, &f1m, &fmm);
    draw_implicit_region(expr, x, y, xm, y1, x2, ym, 0, &fm1, &f2, &fmm, &f2m);
    draw_implicit_region(expr, x, y, x1, ym, xm, y2, 0, &f1m, &fmm, &f3, &fm2);
    draw_implicit_region(expr, x, y, xm, ym, x2, y2, 0, &fmm, &f2m, &fm2, &f4);
}
