#include "display_96_64.h"

#define FIXED_POINT_SHIFT 24
#define MAX_ITER 100

typedef int fixed;

#define FIXED_MUL(a, b) ((fixed)(((long long)(a) * (b)) >> FIXED_POINT_SHIFT))
#define FIXED_DIV(a, b) ((fixed)(((long long)(a) << FIXED_POINT_SHIFT) / (b)))

#define FLOAT_TO_FIXED(x) ((fixed)((x) * (1 << FIXED_POINT_SHIFT)))
// #define FIXED_TO_INT(x) ((x) >> FIXED_POINT_SHIFT)

void draw_mandelbrot()
{
    fixed real_min = FLOAT_TO_FIXED(-2.0), real_max = FLOAT_TO_FIXED(1.0);
    fixed imag_min = FLOAT_TO_FIXED(-1.0), imag_max = FLOAT_TO_FIXED(1.0);

    display_clear();

    for (int y = 0; y < DISPLAY_SIZE_Y; y++)
    {
        for (int x = 0; x < DISPLAY_SIZE_X; x++)
        {
            fixed real = FIXED_MUL(FLOAT_TO_FIXED(x), FIXED_DIV(real_max - real_min, FLOAT_TO_FIXED(DISPLAY_SIZE_X - 1))) + real_min;
            fixed imag = FIXED_MUL(FLOAT_TO_FIXED(y), FIXED_DIV(imag_max - imag_min, FLOAT_TO_FIXED(DISPLAY_SIZE_Y - 1))) + imag_min;

            fixed z_real = 0, z_imag = 0;
            int iter;
            for (iter = 0; iter < MAX_ITER; iter++)
            {
                fixed z_real2 = FIXED_MUL(z_real, z_real), z_imag2 = FIXED_MUL(z_imag, z_imag);

                if (z_real2 + z_imag2 > FLOAT_TO_FIXED(4.0))
                    break;

                fixed z_real_temp = z_real2 - z_imag2 + real;
                z_imag = (FIXED_MUL(z_real, z_imag) << 1) + imag;
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
    draw_mandelbrot();
    return 0;
}
