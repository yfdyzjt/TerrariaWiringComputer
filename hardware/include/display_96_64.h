#ifndef DISPLAY
#define DISPLAY

#define SCREEN ((unsigned char *)_display_96_64_origin)
#define SCREEN_CTRL ((volatile unsigned char *)(_display_96_64_origin + _display_96_64_length - 1))

#define SCREEN_CLC ((unsigned char)0)
#define SCREEN_REF ((unsigned char)1)

#define SCREEN_SIZE_X 96
#define SCREEN_SIZE_Y 64

#endif