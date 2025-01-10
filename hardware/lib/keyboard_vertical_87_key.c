#include "keyboard_vertical_87_key.h"

volatile const unsigned char _keyboard_queue __attribute__((section("keyboard_vertical_87_key.h")));
volatile unsigned char *_keyboard_reset = (volatile unsigned char *)&_keyboard_queue;

void keyboard_reset()
{
    *_keyboard_reset = 1;
}
