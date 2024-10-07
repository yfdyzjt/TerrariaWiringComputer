#include "display_96_64.h"

#define FIXED_POINT_SHIFT 24
#define MAX_ITER 100

typedef int fixed;

#define FIXED_MUL(a, b) ((fixed)(((long long)(a) * (b)) >> FIXED_POINT_SHIFT))
#define FIXED_DIV(a, b) ((fixed)(((long long)(a) << FIXED_POINT_SHIFT) / (b)))

#define FLOAT_TO_FIXED(x) ((fixed)((x) * (1 << FIXED_POINT_SHIFT)))

void draw_julia(fixed c_real, fixed c_imag)
{
    fixed real_min = FLOAT_TO_FIXED(-1.5), real_max = FLOAT_TO_FIXED(1.5);
    fixed imag_min = FLOAT_TO_FIXED(-1.0), imag_max = FLOAT_TO_FIXED(1.0);

    display_clear();

    for (int y = 0; y < DISPLAY_SIZE_Y; y++)
    {
        for (int x = 0; x < DISPLAY_SIZE_X; x++)
        {
            fixed z_real = FIXED_MUL(FLOAT_TO_FIXED(x), FIXED_DIV(real_max - real_min, FLOAT_TO_FIXED(DISPLAY_SIZE_X - 1))) + real_min;
            fixed z_imag = FIXED_MUL(FLOAT_TO_FIXED(y), FIXED_DIV(imag_max - imag_min, FLOAT_TO_FIXED(DISPLAY_SIZE_Y - 1))) + imag_min;

            int iter;
            for (iter = 0; iter < MAX_ITER; iter++)
            {
                fixed z_real2 = FIXED_MUL(z_real, z_real), z_imag2 = FIXED_MUL(z_imag, z_imag);

                if (z_real2 + z_imag2 > FLOAT_TO_FIXED(4.0))
                    break;

                fixed z_real_temp = z_real2 - z_imag2 + c_real;
                z_imag = (FIXED_MUL(z_real, z_imag) << 1) + c_imag;
                z_real = z_real_temp;
            }

            if (iter == MAX_ITER)
            {
                draw_pixel(DISPLAY_SIZE_X - x - 1, y, 1);
                display_refresh();
            }
        }
    }
}

int main()
{
    fixed c_real = FLOAT_TO_FIXED(-0.8);
    fixed c_imag = FLOAT_TO_FIXED(0.156);

    draw_julia(c_real, c_imag);

    return 0;
}
