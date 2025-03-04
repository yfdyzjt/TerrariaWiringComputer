#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "tinyexpr.h"

int main()
{
    char input[256];
    int error_pos;
    double result;

    while (true)
    {
        printf("> ");
        fflush(stdout);

        if (!fgets(input, sizeof(input), stdin))
            break;

        input[strcspn(input, "\n")] = '\0';

        error_pos = 0;
        result = te_interp(input, &error_pos);

        if (error_pos != 0)
        {
            printf("  %s\n", input);
            printf("  %*s^\n", error_pos - 1, "");
            printf("syntax error: %d\n", error_pos);
        }
        else
        {
            printf("= %.15g\n", result);
        }
    }
    return 0;
}
