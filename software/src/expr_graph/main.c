#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "tinyexpr.h"
#include "output.h"
#include "terminal/keyboard_input.h"
#include "terminal/display_output.h"

int main()
{
    char input[128];
    int error;
    double x;
    te_variable vars[] = {{"x", &x}};
    te_expr *expr;

    while (true)
    {
        display_output_nextpage();
        printf("> ");
        fflush(stdout);

        if (!fgets(input, sizeof(input), stdin))
            continue;
        input[strcspn(input, "\n")] = '\0';

        error = 0;
        expr = te_compile(input, vars, sizeof(vars) / sizeof(te_variable), &error);

        if (error != 0)
        {
            printf("  %s\n", input);
            printf("  %*s^\n", error - 1, "");
            printf("syntax error: %d\n", error);
        }
        else
        {
            double center_x, center_y, scale_x, scale_y;
            printf("center x:");
            fflush(stdout);
            scanf("%lf", &center_x);
            printf("center y:");
            fflush(stdout);
            scanf("%lf", &center_y);
            printf("scale x:");
            fflush(stdout);
            scanf("%lf", &scale_x);
            printf("scale y:");
            fflush(stdout);
            scanf("%lf", &scale_y);

            params.center_x = center_x;
            params.center_y = center_y;
            params.scale_x = scale_x;
            params.scale_y = scale_y;

            draw_init(input);
            draw_expr(expr, &x);

            te_free(expr);
            int32_t c;
            while ((c = getchar()) != '\n' && c != EOF)
                ;
            while (true)
                if (keyboard_input_scan() == '\n')
                    break;
        }
    }
    return 0;
}
