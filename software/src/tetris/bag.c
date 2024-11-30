#include "random_number_generator.h"

#include "bag.h"
#include "tetromino.h"

unsigned char pointer;

TetroType bag[TETRO_NUMBER] = {TETRO_J, TETRO_L, TETRO_T, TETRO_S, TETRO_Z, TETRO_I, TETRO_O};

void bag_swap(TetroType *a, TetroType *b)
{
    TetroType temp = *a;
    *a = *b;
    *b = temp;
}

void bag_init()
{
    pointer = 0;

    for (unsigned char i = TETRO_NUMBER - 1; i > 0; i--)
        bag_swap(&bag[i], &bag[rand.u32 % (i + 1)]);
}

TetroType bag_get()
{
    if (pointer >= TETRO_NUMBER)
        bag_init();
    return bag[pointer++];
}
