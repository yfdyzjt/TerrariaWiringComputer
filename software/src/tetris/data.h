#ifndef DATA
#define DATA

extern unsigned int score;
extern unsigned int level;
extern unsigned int lines;

extern unsigned int time_cur;
extern unsigned short time_gap;

void data_init();
void time_init();
void time_update();

void score_add(unsigned int value);
void level_add(unsigned int value);
void lines_add(unsigned int value);

#endif