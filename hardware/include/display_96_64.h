#ifndef DISPLAY
#define DISPLAY

#define DISPLAY_CLC ((unsigned char)0)
#define DISPLAY_REF ((unsigned char)1)

#define DISPLAY_SIZE_X 96
#define DISPLAY_SIZE_Y 64

extern unsigned char _display_buffer[1024];
extern volatile unsigned char *_display_ctrl;

void display_clear();
void display_refresh();

void draw_pixel(int posX, int posY, unsigned char pixel);
void draw_grid(int posX, int posY, int sizeX, int sizeY, unsigned char *grid);
void draw_char(int posX, int posY, char c);
void draw_string(int posX, int posY, char *s);

#endif
