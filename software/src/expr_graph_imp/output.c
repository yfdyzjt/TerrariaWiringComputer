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
#define REAL_TO_SCREEN_X(x) (int32_t)round(GRAPH_POS_X + GRAPH_SIZE_X / 2 - ((x - params.center_x) * params.scale_x))
#define REAL_TO_SCREEN_Y(y) (int32_t)round(GRAPH_POS_Y + GRAPH_SIZE_Y / 2 - ((y - params.center_y) * params.scale_y))

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
    0b0000,
    0b0011,
    0b0101,
    0b0110,
    0b1010,
    0b1001,
    0b0000,
    0b1100,
    0b1100,
    0b0000,
    0b1001,
    0b1010,
    0b0110,
    0b0101,
    0b0011,
    0b0000,
};

static void draw_short_line(int32_t x1, int32_t y1, int32_t x2, int32_t y2)
{
    draw_pixel(x1, y1, 1);
    draw_pixel(x2, y2, 1);

    if (abs(x2 - x1) > 1 || abs(y2 - y1) > 1)
    {
        int32_t xm = (x1 + x2) / 2;
        int32_t ym = (y1 + y2) / 2;
        draw_pixel(xm, ym, 1);
    }
}

static int32_t linear_interpolate(int32_t *p1, int32_t *p2, double *f1, double *f2)
{
    int32_t c;
    double f1_abs = fabs(*f1), f2_abs = fabs(*f2);

    f1_abs = f1_abs < EPSILON ? EPSILON : f1_abs;
    f2_abs = f2_abs < EPSILON ? EPSILON : f2_abs;

    if ((*f1 > 0) != (*f2 > 0))
    {
        double t = f1_abs / (f1_abs + f2_abs);
        c = *p1 + (int32_t)round(t * (*p2 - *p1));
        c = (c < *p1) ? *p1 : ((c > *p2) ? *p2 : c);
    }
    else
        c = (*p1 + *p2) / 2;

    return c;
}

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

    /*
    int32_t cx = (x1 + x2) / 2;
    int32_t cy = (y1 + y2) / 2;
    */

    int32_t cx1 = linear_interpolate(&x1, &x2, f1, f2);
    int32_t cx2 = linear_interpolate(&x1, &x2, f3, f4);
    int32_t cy1 = linear_interpolate(&y1, &y2, f1, f3);
    int32_t cy2 = linear_interpolate(&y1, &y2, f2, f4);

    if (index == 6)
    {
        if (fabs(*f1) + fabs(*f4) < fabs(*f2) + fabs(*f3))
        {
            draw_short_line(cx1, y1, x1, cy1);
            draw_short_line(x2, cy2, cx2, y2);
        }
        else
        {
            draw_short_line(cx1, y1, x2, cy2);
            draw_short_line(x1, cy1, cx2, y2);
        }
    }
    else if (index == 9)
    {
        if (fabs(*f2) + fabs(*f3) < fabs(*f1) + fabs(*f4))
        {
            draw_short_line(cx1, y1, x2, cy2);
            draw_short_line(x1, cy1, cx2, y2);
        }
        else
        {
            draw_short_line(cx1, y1, x1, cy1);
            draw_short_line(x2, cy2, cx2, y2);
        }
    }
    else
    {
        int8_t segments = ms_points[index];
        int8_t i = 0;
        int32_t points[2][2] = {0};

        if (segments & 1)
        {
            points[i][0] = cx1;
            points[i][1] = y1;
            i++;
        }
        if (segments & 2)
        {
            points[i][0] = x1;
            points[i][1] = cy1;
            i++;
        }
        if (segments & 4)
        {
            points[i][0] = x2;
            points[i][1] = cy2;
            i++;
        }
        if (segments & 8)
        {
            points[i][0] = cx2;
            points[i][1] = y2;
            i++;
        }

        draw_short_line(points[0][0], points[0][1], points[1][0], points[1][1]);
    }

    /*
    if (segments & 1)
        draw_pixel(cx, y1, 1);
    if (segments & 2)
        draw_pixel(x1, cy, 1);
    if (segments & 4)
        draw_pixel(x2, cy, 1);
    if (segments & 8)
        draw_pixel(cx, y2, 1);
    if (segments == 0b1001 || segments == 0b0110)
        draw_pixel(cx, cy, 1);
    */

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
