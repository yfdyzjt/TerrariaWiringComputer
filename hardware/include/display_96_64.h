#ifndef DISPLAY
#define DISPLAY

#define DISPLAY_CLC ((unsigned char)0)
#define DISPLAY_REF ((unsigned char)1)

#define DISPLAY_SIZE_X 96
#define DISPLAY_SIZE_Y 64

unsigned char display_buffer[1024] __attribute__((section(__FILE_NAME__)));
volatile unsigned char *display_ctrl = &display_buffer[1023];

#endif
