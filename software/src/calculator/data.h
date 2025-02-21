#ifndef DATA
#define DATA

#include "key.h"

#include <stdint.h>
#include <stdbool.h>

#define MAX_NUMBER_LENGTH 16

extern bool allow_number;
extern bool allow_overwrite;

extern bool is_error;
extern bool is_dot;
extern bool is_neg;
extern bool is_equal;

extern bool mem_have;

extern double dot_exp;

extern int8_t cur_opnd_length;
extern int8_t tar_opnd_length;

extern KeyType bin_op;

extern double opnd_1, opnd_2;
extern double *cur_opnd;

extern double mem_opnd;

void stack_init();
bool stack_is_empty();
bool stack_is_full();
bool stack_push(KeyType key);
bool stack_pop(KeyType *key);

void cur_opnd_init(double *opnd, double value);

#endif
