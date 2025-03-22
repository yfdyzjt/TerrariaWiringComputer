#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "tinyexpr.h"
#include "output.h"
#include "terminal/keyboard_input.h"
#include "terminal/display_output.h"

bool scan_expr(char *input, size_t size);
bool print_error(char *input, int error);
double calculate_integral(te_expr *expr, double *x, double lower, double upper);

int main()
{
    char expr_input[64], lower_input[32], upper_input[32];
    int error;
    double x;
    double lower, upper, result;
    te_variable vars[] = {{"x", &x}};
    te_expr *expr;

    while (true)
    {
        display_output_nextpage();

        printf("f(x): ");
        fflush(stdout);
        if (!scan_expr(expr_input, sizeof(expr_input)))
            continue;
        error = 0;
        expr = te_compile(expr_input, vars, sizeof(vars) / sizeof(te_variable), &error);
        if (print_error(expr_input, error))
            continue;

        printf("lower:");
        fflush(stdout);
        if (!scan_expr(lower_input, sizeof(lower_input)))
            continue;
        error = 0;
        lower = te_interp(lower_input, &error);
        if (print_error(lower_input, error))
            continue;

        printf("upper:");
        fflush(stdout);
        if (!scan_expr(upper_input, sizeof(upper_input)))
            continue;
        error = 0;
        upper = te_interp(upper_input, &error);
        if (print_error(upper_input, error))
            continue;

        draw_init();
        draw_integral_expr(expr_input, lower_input, upper_input);
        result = calculate_integral(expr, &x, lower, upper);
        draw_integral_result(&result);

        te_free(expr);
        while (true)
            if (keyboard_input_scan() == '\n')
                break;
    }
    return 0;
}

bool scan_expr(char *input, size_t size)
{
    if (!fgets(input, size, stdin))
        return false;
    else
    {
        input[strcspn(input, "\n")] = '\0';
        return true;
    }
}

bool print_error(char *input, int error)
{
    if (error != 0)
    {
        printf("     %s\n", input);
        printf("     %*s^\n", error - 1, "");
        printf("syntax error:\"%c\"\n", input[error - 1]);
        return true;
    }
    else
        return false;
}

#define N 15
double calculate_integral(te_expr *expr, double *x, double lower, double upper)
{
    const double xgk[N] = {
        -0.9914553711, -0.9491079123, -0.8648644234,
        -0.7415311856, -0.5860872355, -0.4058451514,
        -0.2077849550, 0.0, 0.2077849550,
        0.4058451514, 0.5860872355, 0.7415311856,
        0.8648644234, 0.9491079123, 0.9914553711};
    const double wgk[N] = {
        0.02293532201, 0.06309209263, 0.1047900103,
        0.1406532597, 0.1690047266, 0.1903505781,
        0.2044329401, 0.2094821411, 0.2044329401,
        0.1903505781, 0.1690047266, 0.1406532597,
        0.1047900103, 0.06309209263, 0.02293532201};

    double a = lower;
    double b = upper;
    double half = (b - a) / 2.0;
    double center = (a + b) / 2.0;

    double result = 0.0;
    for (int32_t i = 0; i < N; i++)
    {
        *x = center + half * xgk[i];
        double fval = te_eval(expr);
        result += wgk[i] * fval;
    }

    result *= half;

    return result;
}
