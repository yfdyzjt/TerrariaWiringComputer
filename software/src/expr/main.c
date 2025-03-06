#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "tinyexpr.h"

int main()
{
    char input[128];
    int error;
    double result;

    while (true)
    {
        printf("> ");
        fflush(stdout);

        if (!fgets(input, sizeof(input), stdin))
            continue;
        input[strcspn(input, "\n")] = '\0';

        error = 0;
        result = te_interp(input, &error);

        if (error != 0)
        {
            printf("  %s\n", input);
            printf("  %*s^\n", error - 1, "");
            printf("syntax error: %d\n", error);
        }
        else
        {
            printf("= %.15g\n", result);
        }
    }
    return 0;
}
