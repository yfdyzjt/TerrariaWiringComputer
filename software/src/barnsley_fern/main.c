#define FIXED_POINT_SHIFT 24

#include "display_96_64.h"
#include "fixed_number.h"
#include "random_number_generator.h"

// Barnsley Fern: 4 affine transformations with weighted probabilities
// f1:  0.00*x + 0.00*y + 0.00      0.00*x + 0.16*y + 0.00      1%
// f2:  0.85*x + 0.04*y + 0.00     -0.04*x + 0.85*y + 1.60     85%
// f3:  0.20*x - 0.26*y + 0.00      0.23*x + 0.22*y + 1.60      7%
// f4: -0.15*x + 0.28*y + 0.00      0.26*x + 0.24*y + 0.44      7%

#define A2 FLOAT_TO_FIXED(0.85)
#define B2 FLOAT_TO_FIXED(0.04)
#define C2 FLOAT_TO_FIXED(-0.04)
#define D2 FLOAT_TO_FIXED(0.85)
#define E2 FLOAT_TO_FIXED(0.00)
#define F2 FLOAT_TO_FIXED(1.60)

#define A3 FLOAT_TO_FIXED(0.20)
#define B3 FLOAT_TO_FIXED(-0.26)
#define C3 FLOAT_TO_FIXED(0.23)
#define D3 FLOAT_TO_FIXED(0.22)
#define E3 FLOAT_TO_FIXED(0.00)
#define F3 FLOAT_TO_FIXED(1.60)

#define A4 FLOAT_TO_FIXED(-0.15)
#define B4 FLOAT_TO_FIXED(0.28)
#define C4 FLOAT_TO_FIXED(0.26)
#define D4 FLOAT_TO_FIXED(0.24)
#define E4 FLOAT_TO_FIXED(0.00)
#define F4 FLOAT_TO_FIXED(0.44)

// Screen: fern y-axis (growth, 0~10) -> screen x (0~95), fern x-axis (spread, -2.5~2.5) -> screen y (0~63)
#define Y_TO_X_SCALE FLOAT_TO_FIXED(9.6)    // 96 / 10
#define X_TO_Y_SCALE FLOAT_TO_FIXED(12.8)   // 64 / 5

int main()
{
    fixed x = 0, y = 0;

    while (1)
    {
        unsigned char r = rand.u8;
        fixed nx, ny;

        if (r < 2)      // 1%
        {
            nx = 0;
            ny = FIXED_MUL(FLOAT_TO_FIXED(0.16), y);
        }
        else if (r < 219)  // 85%
        {
            nx = FIXED_MUL(A2, x) + FIXED_MUL(B2, y) + E2;
            ny = FIXED_MUL(C2, x) + FIXED_MUL(D2, y) + F2;
        }
        else if (r < 237)  // 7%
        {
            nx = FIXED_MUL(A3, x) + FIXED_MUL(B3, y) + E3;
            ny = FIXED_MUL(C3, x) + FIXED_MUL(D3, y) + F3;
        }
        else               // 7%
        {
            nx = FIXED_MUL(A4, x) + FIXED_MUL(B4, y) + E4;
            ny = FIXED_MUL(C4, x) + FIXED_MUL(D4, y) + F4;
        }

        x = nx;
        y = ny;

        int px = FIXED_TO_INT(FIXED_MUL(y, Y_TO_X_SCALE));
        int py = DISPLAY_SIZE_Y / 2 + FIXED_TO_INT(FIXED_MUL(x, X_TO_Y_SCALE));

        draw_pixel(px, py, 1);
        display_refresh();
    }

    return 0;
}
