#ifndef INPUT
#define INPUT

#include "error.h"
#include "key.h"

#define KEY_NUMBER_X 7
#define KEY_NUMBER_Y 4

extern const KeyType key_pos[KEY_NUMBER_X][KEY_NUMBER_Y];

ErrorType cursor_update();

#endif
