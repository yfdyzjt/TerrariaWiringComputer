#include "display_96_64.h"

#include "cos_table.h"
#include "sin_table.h"

#define FIXED_POINT_SHIFT 24
#define DISPLAY_CENTER_X (DISPLAY_SIZE_X / 2)
#define DISPLAY_CENTER_Y (DISPLAY_SIZE_Y / 2)

typedef int fixed;

#define FIXED_MUL(a, b) ((fixed)(((long long)(a) * (b)) >> FIXED_POINT_SHIFT))
#define FIXED_DIV(a, b) ((fixed)(((long long)(a) << FIXED_POINT_SHIFT) / (b)))

#define FLOAT_TO_FIXED(x) ((fixed)((x) * (1 << FIXED_POINT_SHIFT)))
#define FIXED_TO_INT(x) ((x) >> FIXED_POINT_SHIFT)

fixed fixed_sin(int angle)
{
    return (fixed)sin_table[angle % 360];
}

fixed fixed_cos(int angle)
{
    return (fixed)cos_table[angle % 360];
}

fixed cube_vertices[8][3] = {
    {FLOAT_TO_FIXED(1), FLOAT_TO_FIXED(1), FLOAT_TO_FIXED(1)},
    {FLOAT_TO_FIXED(-1), FLOAT_TO_FIXED(1), FLOAT_TO_FIXED(1)},
    {FLOAT_TO_FIXED(-1), FLOAT_TO_FIXED(-1), FLOAT_TO_FIXED(1)},
    {FLOAT_TO_FIXED(1), FLOAT_TO_FIXED(-1), FLOAT_TO_FIXED(1)},
    {FLOAT_TO_FIXED(1), FLOAT_TO_FIXED(1), FLOAT_TO_FIXED(-1)},
    {FLOAT_TO_FIXED(-1), FLOAT_TO_FIXED(1), FLOAT_TO_FIXED(-1)},
    {FLOAT_TO_FIXED(-1), FLOAT_TO_FIXED(-1), FLOAT_TO_FIXED(-1)},
    {FLOAT_TO_FIXED(1), FLOAT_TO_FIXED(-1), FLOAT_TO_FIXED(-1)}};

void project(fixed x, fixed y, fixed z, int *screen_x, int *screen_y)
{
    fixed distance = FLOAT_TO_FIXED(4);
    fixed inv_z = FIXED_DIV(FLOAT_TO_FIXED(1), z + distance);

    *screen_x = DISPLAY_CENTER_X + FIXED_TO_INT(FIXED_MUL(x, inv_z) * DISPLAY_CENTER_Y);
    *screen_y = DISPLAY_CENTER_Y - FIXED_TO_INT(FIXED_MUL(y, inv_z) * DISPLAY_CENTER_Y);
}

void draw_cube(int angle_x, int angle_y, int angle_z)
{
    fixed cos_x = fixed_cos(angle_x);
    fixed sin_x = fixed_sin(angle_x);
    fixed cos_y = fixed_cos(angle_y);
    fixed sin_y = fixed_sin(angle_y);
    fixed cos_z = fixed_cos(angle_z);
    fixed sin_z = fixed_sin(angle_z);

    fixed rotation_matrix[3][3] = {
        {FIXED_MUL(cos_y, cos_z), FIXED_MUL(cos_y, sin_z), -sin_y},
        {FIXED_MUL(sin_x, FIXED_MUL(sin_y, cos_z)) - FIXED_MUL(cos_x, sin_z),
         FIXED_MUL(sin_x, FIXED_MUL(sin_y, sin_z)) + FIXED_MUL(cos_x, cos_z),
         FIXED_MUL(sin_x, cos_y)},
        {FIXED_MUL(cos_x, FIXED_MUL(sin_y, cos_z)) + FIXED_MUL(sin_x, sin_z),
         FIXED_MUL(cos_x, FIXED_MUL(sin_y, sin_z)) - FIXED_MUL(sin_x, cos_z),
         FIXED_MUL(cos_x, cos_y)}};

    int projected[8][2];
    for (int i = 0; i < 8; i++)
    {
        fixed x = cube_vertices[i][0];
        fixed y = cube_vertices[i][1];
        fixed z = cube_vertices[i][2];

        fixed new_x = FIXED_MUL(rotation_matrix[0][0], x) + FIXED_MUL(rotation_matrix[0][1], y) + FIXED_MUL(rotation_matrix[0][2], z);
        fixed new_y = FIXED_MUL(rotation_matrix[1][0], x) + FIXED_MUL(rotation_matrix[1][1], y) + FIXED_MUL(rotation_matrix[1][2], z);
        fixed new_z = FIXED_MUL(rotation_matrix[2][0], x) + FIXED_MUL(rotation_matrix[2][1], y) + FIXED_MUL(rotation_matrix[2][2], z);

        project(new_x, new_y, new_z, &projected[i][0], &projected[i][1]);
    }

    int edges[12][2] = {
        {0, 1}, {1, 2}, {2, 3}, {3, 0}, {4, 5}, {5, 6}, {6, 7}, {7, 4}, {0, 4}, {1, 5}, {2, 6}, {3, 7}};

    for (int i = 0; i < 12; i++)
    {
        draw_line(projected[edges[i][0]][0], projected[edges[i][0]][1],
                  projected[edges[i][1]][0], projected[edges[i][1]][1], solid_line_one_func);
    }
}

int main()
{
    int angle_x = 0, angle_y = 0, angle_z = 0;

    while (1)
    {
        display_clear();
        draw_cube(angle_x, angle_y, angle_z);
        display_refresh();

        angle_x = (angle_x + 1) % 360;
        angle_y = (angle_y + 1) % 360;
        angle_z = (angle_z + 1) % 360;
    }

    return 0;
}
