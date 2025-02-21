#include "error.h"
#include "input.h"
#include "output.h"
#include "data.h"

#include <stdbool.h>

int main()
{
    draw_init();
    cur_opnd_init(&opnd_1, 0);

    while (true)
    {
        ErrorType err = cursor_update();
        error(err);
    }
}
