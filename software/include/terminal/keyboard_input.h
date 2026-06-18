#ifdef TERMINAL
#ifndef KEYBOARD_INPUT
#define KEYBOARD_INPUT

#include <stdbool.h>

typedef enum
{
    MK_SHIFT = 0b00000001,
    MK_CTRL = 0b00000010,
    MK_ALT = 0b00000100,
    MK_WIN = 0b00001000,
    MK_FN = 0b00010000,
    MK_CAPS_LOCK = 0b00100000,
} ModifierKey;

void keyboard_input_init();
char keyboard_input_scan();
bool keyboard_input_test(ModifierKey mk);

#endif
#endif
