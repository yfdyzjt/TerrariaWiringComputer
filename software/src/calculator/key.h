#ifndef KEY
#define KEY

#include "error.h"

typedef enum
{
    KEY_0,
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,
    KEY_9,
    KEY_DOT,
    KEY_NEG,

    KEY_RECIP,
    KEY_SQ,
    KEY_SQRT,

    KEY_ADD,
    KEY_SUB,
    KEY_MUL,
    KEY_DIV,

    KEY_EQ,
    KEY_BACK,
    KEY_CE,
    KEY_C,

    KEY_MR,
    KEY_MC,
    KEY_MS,
    KEY_MADD,
    KEY_MSUB,

    KEY_NONE,
    KEY_NUMBER,
} KeyType;

extern const ErrorType (*key_press[KEY_NUMBER])(KeyType);

#endif
