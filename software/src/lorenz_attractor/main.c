#define FIXED_POINT_SHIFT 20

#include "display_96_64.h"
#include "fixed_number.h"

// Lorenz system: dx/dt = sigma*(y-x), dy/dt = x*(rho-z)-y, dz/dt = x*y - beta*z
// Classic parameters: sigma=10, rho=28, beta=8/3
#define SIGMA FLOAT_TO_FIXED(10.0)
#define RHO   FLOAT_TO_FIXED(28.0)
#define BETA  FLOAT_TO_FIXED(2.6666666667)

#define DT    FLOAT_TO_FIXED(0.01)

// Screen: X-Z plane, x ~[-20,20] -> [0,95], z ~[0,50] -> [63,0]
#define X_SCALE FLOAT_TO_FIXED(2.0)     // 96/48
#define Z_SCALE FLOAT_TO_FIXED(1.28)    // 64/50

int main()
{
    fixed x = FLOAT_TO_FIXED(1.0);
    fixed y = FLOAT_TO_FIXED(1.0);
    fixed z = FLOAT_TO_FIXED(1.0);

    while (1)
    {
        fixed dx = FIXED_MUL(SIGMA, y - x);
        fixed dy = FIXED_MUL(x, RHO - z) - y;
        fixed dz = FIXED_MUL(x, y) - FIXED_MUL(BETA, z);

        x += FIXED_MUL(dx, DT);
        y += FIXED_MUL(dy, DT);
        z += FIXED_MUL(dz, DT);

        int px = DISPLAY_SIZE_X / 2 + FIXED_TO_INT(FIXED_MUL(x, X_SCALE));
        int py = DISPLAY_SIZE_Y - 1 - FIXED_TO_INT(FIXED_MUL(z, Z_SCALE));

        draw_pixel(px, py, 1);
        display_refresh();
    }

    return 0;
}
