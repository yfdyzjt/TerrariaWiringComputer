#ifndef DISPLAY
#define DISPLAY

#define DISPLAY_CLC ((unsigned char)0)
#define DISPLAY_REF ((unsigned char)1)

#define DISPLAY_SIZE_X 96
#define DISPLAY_SIZE_Y 64

extern unsigned char _display_buffer[1024];
extern volatile unsigned char *_display_ctrl;

typedef int (*line_evolution_func)(int step, int x, int y);

void display_clear();
void display_refresh();

void draw_pixel(int x, int y, unsigned char pixel);
void draw_line(int x0, int y0, int x1, int y1, line_evolution_func evolve);
void draw_rect(int x, int y, int w, int h, line_evolution_func evolve);
void draw_grid_char(int x, int y, int w, int h, const unsigned char *grid);
void draw_grid_short(int x, int y, int w, int h, const unsigned short *grid);
void draw_grid_int(int x, int y, int w, int h, const unsigned int *grid);
void draw_char(int x, int y, char c);
void draw_string(int x, int y, char *s);

int solid_line_zero_func(int step, int x, int y);
int solid_line_one_func(int step, int x, int y);
int dotted_line_func(int step, int x, int y);

#endif
