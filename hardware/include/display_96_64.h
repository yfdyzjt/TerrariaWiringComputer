#ifndef DISPLAY
#define DISPLAY

#define DISPLAY_CLC ((unsigned char)0)
#define DISPLAY_REF ((unsigned char)1)

#define DISPLAY_SIZE_X 96
#define DISPLAY_SIZE_Y 64

unsigned char _display_buffer[1024] __attribute__((section(__FILE_NAME__)));
volatile unsigned char *_display_ctrl = &_display_buffer[1023];

#endif
