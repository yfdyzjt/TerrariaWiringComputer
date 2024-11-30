#ifndef SCORE
#define SCORE

extern signed char is_t_spin;
extern signed char is_mini_t_spin;
extern signed char is_perfect_clear;

void score_soft_drop();
void score_hard_drop(signed char dy);
void score_lock(unsigned char clear_lines);

void score_init();

#endif
