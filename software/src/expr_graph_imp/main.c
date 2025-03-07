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
    char processed_input[128];
    int error;
    double x, y;
    te_variable vars[] = {{"x", &x}, {"y", &y}};
    te_expr *expr;

    while (true)
    {
        printf("> ");
        fflush(stdout);

        if (!fgets(input, sizeof(input), stdin))
            continue;
        input[strcspn(input, "\n")] = '\0';
        char *equals_sign = strchr(input, '=');
        if (equals_sign == NULL)
        {
            strcpy(processed_input, input);
            strcat(input, "=0");
        }
        else
        {
            strncpy(processed_input, input, equals_sign - input);
            processed_input[equals_sign - input] = '\0';

            strcat(processed_input, "-(");
            strcat(processed_input, equals_sign + 1);
            strcat(processed_input, ")");
        }

        error = 0;
        expr = te_compile(processed_input, vars, sizeof(vars) / sizeof(te_variable), &error);

        if (error != 0)
        {
            printf("  %s\n", processed_input);
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

            int32_t c;
            while ((c = getchar()) != '\n' && c != EOF)
                ;

            params.center_x = center_x;
            params.center_y = center_y;
            params.scale_x = scale_x;
            params.scale_y = scale_y;

            draw_init(input);
            draw_implicit_expr(expr, &x, &y);

            while (true)
                if (keyboard_input_scan() == '\n')
                    break;

            display_output_nextpage();
            te_free(expr);
        }
    }
    return 0;
}
