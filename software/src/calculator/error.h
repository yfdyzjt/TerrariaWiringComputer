#ifndef ERROR
#define ERROR

typedef enum
{
    ERROR_NO_KEY,
    ERROR_DIV_ZERO,
    ERROR_SQRT_NEG,

    ERROR_NONE,
    ERROR_NUMBER,
} ErrorType;

void error(ErrorType err);

#endif
