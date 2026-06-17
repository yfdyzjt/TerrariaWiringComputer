#define FIXED_POINT_SHIFT 24

#include "display_96_64.h"
#include "fixed_number.h"

// Koch snowflake: recursively subdivide each segment into 4 smaller segments.
// The middle third is replaced with two sides of an equilateral triangle (bump).
#define COS60      FLOAT_TO_FIXED(0.5)
#define SIN60      FLOAT_TO_FIXED(0.86602540378)
#define ONE_THIRD  FLOAT_TO_FIXED(0.33333333333)

#define MAX_DEPTH 3

// Equilateral triangle: height=48, top y=0, base y=47
#define V_TOP_X 48
#define V_TOP_Y 0
#define V_LFT_X 20
#define V_LFT_Y 47
#define V_RGT_X 76
#define V_RGT_Y 47

void koch_curve(fixed x1, fixed y1, fixed x2, fixed y2, int depth)
{
    if (depth == 0)
    {
        draw_line(FIXED_TO_INT(x1), FIXED_TO_INT(y1),
                  FIXED_TO_INT(x2), FIXED_TO_INT(y2),
                  solid_line_one_func);
        display_refresh();
        return;
    }

    fixed vx = x2 - x1;
    fixed vy = y2 - y1;

    fixed tx = FIXED_MUL(vx, ONE_THIRD);
    fixed ty = FIXED_MUL(vy, ONE_THIRD);

    // C = A + V/3
    fixed cx = x1 + tx;
    fixed cy = y1 + ty;

    // D = A + 2*V/3
    fixed dx = x1 + tx * 2;
    fixed dy = y1 + ty * 2;

    // E = C + rotate(V/3, +60 deg)
    fixed rx = FIXED_MUL(tx, COS60) - FIXED_MUL(ty, SIN60);
    fixed ry = FIXED_MUL(tx, SIN60) + FIXED_MUL(ty, COS60);

    fixed ex = cx + rx;
    fixed ey = cy + ry;

    koch_curve(x1, y1, cx, cy, depth - 1);
    koch_curve(cx, cy, ex, ey, depth - 1);
    koch_curve(ex, ey, dx, dy, depth - 1);
    koch_curve(dx, dy, x2, y2, depth - 1);
}

int main()
{
    fixed v1x = FLOAT_TO_FIXED(V_TOP_X), v1y = FLOAT_TO_FIXED(V_TOP_Y);
    fixed v2x = FLOAT_TO_FIXED(V_LFT_X), v2y = FLOAT_TO_FIXED(V_LFT_Y);
    fixed v3x = FLOAT_TO_FIXED(V_RGT_X), v3y = FLOAT_TO_FIXED(V_RGT_Y);

    for (int depth = 0; depth <= MAX_DEPTH; depth++)
    {
        display_clear();

        koch_curve(v1x, v1y, v2x, v2y, depth);
        koch_curve(v2x, v2y, v3x, v3y, depth);
        koch_curve(v3x, v3y, v1x, v1y, depth);
    }

    return 0;
}
