#ifndef KEYBOARD
#define KEYBOARD

extern volatile const unsigned char _keyboard_queue;
extern volatile unsigned char *_keyboard_reset;

void keyboard_reset();

#endif
