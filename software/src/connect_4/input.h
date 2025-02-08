#ifndef INPUT_H
#define INPUT_H

#include <stdint.h>

#include "data.h"

uint32_t random_number_get();

void key_wait();
void key_refresh();
bool key_j();
bool key_l();
bool key_r();

#endif
