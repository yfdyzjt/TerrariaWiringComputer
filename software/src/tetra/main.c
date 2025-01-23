#define FIXED_POINT_SHIFT 24
#include "display_96_64.h"
#include "cos_table.h"
#include "sin_table.h"
#include "fixed_number.h"

#define DISPLAY_CENTER_X (DISPLAY_SIZE_X / 2)
#define DISPLAY_CENTER_Y (DISPLAY_SIZE_Y / 2)

fixed fixed_sin(int angle)
{
    return (fixed)sin_table[angle % 360];
}

fixed fixed_cos(int angle)
{
    return (fixed)cos_table[angle % 360];
}

fixed tetrahedron_vertices[4][3] = {
    {FLOAT_TO_FIXED(0), FLOAT_TO_FIXED(2), FLOAT_TO_FIXED(-0.7071067812)},
    {FLOAT_TO_FIXED(-1.732050808), FLOAT_TO_FIXED(-1), FLOAT_TO_FIXED(-0.7071067812)},
    {FLOAT_TO_FIXED(1.732050808), FLOAT_TO_FIXED(-1), FLOAT_TO_FIXED(-0.7071067812)},
    {FLOAT_TO_FIXED(0), FLOAT_TO_FIXED(0), FLOAT_TO_FIXED(2.121320344)}};

int faces[4][3] = {
    {0, 1, 2}, {0, 2, 3}, {0, 3, 1}, {1, 3, 2}};

void project(fixed x, fixed y, fixed z, int *screen_x, int *screen_y)
{
    fixed scale = FLOAT_TO_FIXED(2);
    *screen_x = DISPLAY_CENTER_X + FIXED_TO_INT(FIXED_DIV(x, scale) * DISPLAY_CENTER_Y);
    *screen_y = DISPLAY_CENTER_Y - FIXED_TO_INT(FIXED_DIV(y, scale) * DISPLAY_CENTER_Y);
}

void calculate_normal(fixed v1[3], fixed v2[3], fixed v3[3], fixed normal[3])
{
    fixed u[3] = {v2[0] - v1[0], v2[1] - v1[1], v2[2] - v1[2]};
    fixed v[3] = {v3[0] - v1[0], v3[1] - v1[1], v3[2] - v1[2]};
    normal[0] = FIXED_MUL(u[1], v[2]) - FIXED_MUL(u[2], v[1]);
    normal[1] = FIXED_MUL(u[2], v[0]) - FIXED_MUL(u[0], v[2]);
    normal[2] = FIXED_MUL(u[0], v[1]) - FIXED_MUL(u[1], v[0]);
}

void draw_tetrahedron(int angle_x, int angle_y, int angle_z)
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

    fixed transformed[4][3];
    int projected[4][2];

    for (int i = 0; i < 4; i++)
    {
        fixed x = tetrahedron_vertices[i][0];
        fixed y = tetrahedron_vertices[i][1];
        fixed z = tetrahedron_vertices[i][2];

        transformed[i][0] = FIXED_MUL(rotation_matrix[0][0], x) +
                            FIXED_MUL(rotation_matrix[0][1], y) +
                            FIXED_MUL(rotation_matrix[0][2], z);
        transformed[i][1] = FIXED_MUL(rotation_matrix[1][0], x) +
                            FIXED_MUL(rotation_matrix[1][1], y) +
                            FIXED_MUL(rotation_matrix[1][2], z);
        transformed[i][2] = FIXED_MUL(rotation_matrix[2][0], x) +
                            FIXED_MUL(rotation_matrix[2][1], y) +
                            FIXED_MUL(rotation_matrix[2][2], z);

        project(transformed[i][0], transformed[i][1], transformed[i][2], &projected[i][0], &projected[i][1]);
    }

    for (int i = 0; i < 4; i++)
    {
        fixed normal[3];
        calculate_normal(transformed[faces[i][0]], transformed[faces[i][1]], transformed[faces[i][2]], normal);

        if (FIXED_MUL(normal[2], FLOAT_TO_FIXED(-1)) > 0)
        {
            for (int j = 0; j < 3; j++)
            {
                int start = faces[i][j];
                int end = faces[i][(j + 1) % 3];
                draw_line(projected[start][0], projected[start][1], projected[end][0], projected[end][1], solid_line_one_func);
            }
        }
    }
}

int main()
{
    int angle_x = 0, angle_y = 0, angle_z = 0;
    while (1)
    {
        display_clear();
        draw_tetrahedron(angle_x, angle_y, angle_z);
        display_refresh();
        angle_x = (angle_x + 1) % 360;
        angle_y = (angle_y + 1) % 360;
        angle_z = (angle_z + 1) % 360;
    }
    return 0;
}
