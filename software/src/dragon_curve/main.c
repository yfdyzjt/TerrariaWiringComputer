#include "display_96_64.h"

// Heighway dragon curve: turtle graphics with bit-formula turns.
// At segment n (1-indexed), turn right if ((n & -n) << 1) & n, else turn left.
#define MAX_DEPTH 10
#define STEP      2

int main()
{
    int num = 1 << MAX_DEPTH;

    int x = 75, y = 20;  // start top-center
    int nx = x, ny = y;
    int dir = 1;         // 0=east, 1=south, 2=west, 3=north

    for (int i = 0; i < num; i++)
    {
        // Turn (skip for first segment)
        if (i > 0)
        {
            int n = i;
            if ((((n & -n) << 1) & n) != 0)
                dir = (dir + 3) & 3;  // left
            else
                dir = (dir + 1) & 3;  // right
        }

        // Move one step in current direction
        switch (dir)
        {
        case 0: nx += STEP; break;
        case 1: ny += STEP; break;
        case 2: nx -= STEP; break;
        case 3: ny -= STEP; break;
        }

        draw_line(x, y, nx, ny, solid_line_one_func);
        display_refresh();

        x = nx;
        y = ny;
    }

    return 0;
}
