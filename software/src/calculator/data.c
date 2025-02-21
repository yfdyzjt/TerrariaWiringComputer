#include "data.h"
#include "key.h"
#include "output.h"

#include <stdbool.h>
#include <stdint.h>

typedef struct
{
    KeyType data[MAX_NUMBER_LENGTH];
    int8_t top;
} Stack;

static Stack stack;

bool allow_number = false;
bool allow_overwrite = true;

bool is_error = false;
bool is_dot = false;
bool is_neg = false;
bool is_equal = false;

double dot_exp = 1.0;

int8_t cur_opnd_length = 0;
int8_t tar_opnd_length = 0;

KeyType bin_op = KEY_NONE;

double opnd_1 = 0.0, opnd_2 = 0.0;
double *cur_opnd = &opnd_1;

bool mem_have = false;

double mem_opnd = 0.0;

extern void stack_init();
extern bool stack_is_empty();
extern bool stack_is_full();
extern bool stack_push(KeyType key);
extern bool stack_pop(KeyType *key);

void cur_opnd_init(double *opnd, double value)
{
    allow_number = false;
    allow_overwrite = true;

    is_error = false;
    is_dot = false;
    is_neg = false;

    dot_exp = 1.0;

    *opnd = value;
    cur_opnd = opnd;

    cur_opnd_length = 0;
    stack_init();
    result_draw_full(*cur_opnd);
}

void stack_init()
{
    stack.top = 0;
}

bool stack_is_empty()
{
    return stack.top == 0;
}

bool stack_is_full()
{
    return stack.top == MAX_NUMBER_LENGTH;
}

bool stack_push(KeyType key)
{
    if (stack_is_full(stack))
        return false;
    else
    {
        stack.data[stack.top++] = key;
        return true;
    }
}

bool stack_pop(KeyType *key)
{
    if (stack_is_empty(stack))
        return false;
    else
    {
        *key = stack.data[--stack.top];
        return true;
    }
}
