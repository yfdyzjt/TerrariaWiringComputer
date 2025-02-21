#ifndef OUTPUT
#define OUTPUT

#include <stdint.h>

void draw_init();

void cursor_draw(int8_t x, int8_t y);
void cursor_erase(int8_t x, int8_t y);

void result_draw_ps(const char *prefix, const char *suffix);
void result_erase_ps(uint8_t prefix_len, uint8_t suffix_len);
void result_draw_full(double result);
void result_draw_clear();

void operand_draw_ps(const char *prefix, const char *suffix);
void operand_erase_ps(uint8_t prefix_len, uint8_t suffix_len);
void operand_draw_full(double result);
void operand_draw_clear();
void operand_cur_draw_clear();

void error_draw(const char *str);

#endif
