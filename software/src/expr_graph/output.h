#ifndef OUTPUT
#define OUTPUT

#include "tinyexpr.h"

typedef struct
{
    double center_x;
    double center_y;
    double scale_x;
    double scale_y;
} expr_graph_params;

extern expr_graph_params params;

void draw_expr(te_expr *expr, double *x);
void draw_init(const char *input);

#endif
