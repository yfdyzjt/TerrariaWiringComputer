#include "error.h"
#include "data.h"
#include "output.h"

#include <stdbool.h>

const char *error_str[ERROR_NUMBER] = {
    [ERROR_NO_KEY] = "No key",
    [ERROR_DIV_ZERO] = "Divide zero",
    [ERROR_SQRT_NEG] = "Sqrt negative",
};

void error(ErrorType err)
{
    if (err == ERROR_NONE)
        return;

    is_error = true;

    error_draw(error_str[err]);
}
