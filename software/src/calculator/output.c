#include "output.h"
#include "data.h"
#include "input.h"

#include "display_96_64.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define CURSOR_WIDTH 13
#define CURSOR_HEIGHT 13

#define CURSOR_OFFSET_X 0
#define CURSOR_OFFSET_Y 16

#define KEY_WIDTH 9
#define KEY_HEIGHT 9

#define KEY_STEP_X (CURSOR_WIDTH - 1)
#define KEY_STEP_Y (CURSOR_HEIGHT - 1)

#define KEY_OFFSET_X (CURSOR_OFFSET_X + (CURSOR_WIDTH - KEY_WIDTH) / 2)
#define KEY_OFFSET_Y (CURSOR_OFFSET_Y + (CURSOR_HEIGHT - KEY_HEIGHT) / 2)

#define CHARACTER_WIDTH 5
#define CHARACTER_HEIGHT 7

#define OPERAND_OFFSET_X 0
#define OPERAND_OFFSET_Y 0

#define RESULT_OFFSET_X 0
#define RESULT_OFFSET_Y 8

#define MAX_DISPLAY_LENGTH (DISPLAY_SIZE_X / (CHARACTER_WIDTH + 1))
#define MAX_SHIFT_LENGTH (32 / (CHARACTER_WIDTH + 1))

static const uint16_t key_grid[KEY_NUMBER][KEY_HEIGHT] = {
    [KEY_0] = {
        0b000000000,
        0b000111000,
        0b001000100,
        0b001001100,
        0b001010100,
        0b001100100,
        0b001000100,
        0b000111000,
        0b000000000,
    },
    [KEY_1] = {
        0b000000000,
        0b000010000,
        0b000110000,
        0b000010000,
        0b000010000,
        0b000010000,
        0b000010000,
        0b000111000,
        0b000000000,
    },
    [KEY_2] = {
        0b000000000,
        0b000111000,
        0b001000100,
        0b000000100,
        0b000001000,
        0b000010000,
        0b000100000,
        0b001111100,
        0b000000000,
    },
    [KEY_3] = {
        0b000000000,
        0b000111000,
        0b001000100,
        0b000000100,
        0b000011000,
        0b000000100,
        0b001000100,
        0b000111000,
    },
    [KEY_4] = {
        0b000000000,
        0b000001000,
        0b000011000,
        0b000101000,
        0b001001000,
        0b001111100,
        0b000001000,
        0b000001000,
        0b000000000,
    },
    [KEY_5] = {
        0b000000000,
        0b001111100,
        0b001000000,
        0b001111000,
        0b000000100,
        0b000000100,
        0b001000100,
        0b000111000,
        0b000000000,
    },
    [KEY_6] = {
        0b000000000,
        0b000111000,
        0b001000000,
        0b001000000,
        0b001111000,
        0b001000100,
        0b001000100,
        0b000111000,
        0b000000000,
    },
    [KEY_7] = {
        0b000000000,
        0b001111100,
        0b000000100,
        0b000001000,
        0b000010000,
        0b000010000,
        0b000010000,
        0b000000000,
    },
    [KEY_8] = {
        0b000000000,
        0b000111000,
        0b001000100,
        0b001000100,
        0b000111000,
        0b001000100,
        0b001000100,
        0b000111000,
        0b000000000,
    },
    [KEY_9] = {
        0b000000000,
        0b000111000,
        0b001000100,
        0b001000100,
        0b000111100,
        0b000000100,
        0b000000100,
        0b000111000,
        0b000000000,
    },
    [KEY_DOT] = {
        0b000000000,
        0b000000000,
        0b000000000,
        0b000000000,
        0b000000000,
        0b000000000,
        0b000000000,
        0b000010000,
        0b000000000,
    },
    [KEY_NEG] = {
        0b000000000,
        0b001000000,
        0b011100100,
        0b001001000,
        0b000010000,
        0b000100000,
        0b001001110,
        0b000000000,
        0b000000000,
    },

    [KEY_RECIP] = {
        0b010000000,
        0b110000010,
        0b010000100,
        0b111001000,
        0b000010000,
        0b000100000,
        0b001000101,
        0b010000010,
        0b000000101,
    },
    [KEY_SQ] = {
        0b000000110,
        0b000000001,
        0b000000010,
        0b000000100,
        0b100010111,
        0b010100000,
        0b001000000,
        0b010100000,
        0b100010000,
    },
    [KEY_SQRT] = {
        0b000000000,
        0b001111111,
        0b001000000,
        0b001010001,
        0b001001010,
        0b010000100,
        0b110001010,
        0b010010001,
        0b000000000,
    },
    [KEY_ADD] = {
        0b000000000,
        0b000000000,
        0b000010000,
        0b000010000,
        0b001111100,
        0b000010000,
        0b000010000,
        0b000000000,
        0b000000000,
    },
    [KEY_SUB] = {
        0b000000000,
        0b000000000,
        0b000000000,
        0b000000000,
        0b001111100,
        0b000000000,
        0b000000000,
        0b000000000,
        0b000000000,
    },
    [KEY_MUL] = {
        0b000000000,
        0b000000000,
        0b001000100,
        0b000101000,
        0b000010000,
        0b000101000,
        0b001000100,
        0b000000000,
        0b000000000,
    },
    [KEY_DIV] = {
        0b000000000,
        0b000000000,
        0b000010000,
        0b000000000,
        0b001111100,
        0b000000000,
        0b000010000,
        0b000000000,
        0b000000000,
    },
    [KEY_EQ] = {
        0b000000000,
        0b000000000,
        0b000000000,
        0b001111100,
        0b000000000,
        0b001111100,
        0b000000000,
        0b000000000,
        0b000000000,
    },
    [KEY_BACK] = {
        0b000000000,
        0b000111111,
        0b001000001,
        0b010010101,
        0b100001001,
        0b010010101,
        0b001000001,
        0b000111111,
        0b000000000,
    },
    [KEY_CE] = {
        0b000000000,
        0b000000000,
        0b001101110,
        0b010001000,
        0b010001100,
        0b010001000,
        0b001101110,
        0b000000000,
        0b000000000,
    },
    [KEY_C] = {
        0b000000000,
        0b000111000,
        0b001000100,
        0b001000000,
        0b001000000,
        0b001000000,
        0b001000100,
        0b000111000,
        0b000000000,
    },
    [KEY_MR] = {
        0b000000000,
        0b000000000,
        0b011100110,
        0b101010101,
        0b101010110,
        0b101010101,
        0b101010101,
        0b000000000,
        0b000000000,
    },
    [KEY_MC] = {
        0b000000000,
        0b000000000,
        0b011100011,
        0b101010100,
        0b101010100,
        0b101010100,
        0b101010011,
        0b000000000,
        0b000000000,
    },
    [KEY_MS] = {
        0b000000000,
        0b000000000,
        0b011100011,
        0b101010100,
        0b101010010,
        0b101010001,
        0b101010110,
        0b000000000,
        0b000000000,
    },
    [KEY_MADD] = {
        0b000000000,
        0b000000000,
        0b011100000,
        0b101010010,
        0b101010111,
        0b101010010,
        0b101010000,
        0b000000000,
        0b000000000,
    },
    [KEY_MSUB] = {
        0b000000000,
        0b000000000,
        0b011100000,
        0b101010000,
        0b101010111,
        0b101010000,
        0b101010000,
        0b000000000,
        0b000000000,
    },
    [KEY_NONE] = {
        0b000000000,
        0b000000000,
        0b000000000,
        0b000000000,
        0b000000000,
        0b000000000,
        0b000000000,
        0b000000000,
    }};

static void cursor_draw_erase(int8_t x, int8_t y, uint8_t pixel)
{
    uint32_t dx = CURSOR_OFFSET_X + x * (CURSOR_WIDTH - 1);
    uint32_t dy = CURSOR_OFFSET_Y + y * (CURSOR_HEIGHT - 1);

    draw_pixel(dx + 0, dy + 0, pixel);
    draw_pixel(dx + 1, dy + 0, pixel);
    draw_pixel(dx + 2, dy + 0, pixel);
    draw_pixel(dx + 0, dy + 1, pixel);
    draw_pixel(dx + 0, dy + 2, pixel);

    draw_pixel(dx + CURSOR_WIDTH - 1, dy + 0, pixel);
    draw_pixel(dx + CURSOR_WIDTH - 2, dy + 0, pixel);
    draw_pixel(dx + CURSOR_WIDTH - 3, dy + 0, pixel);
    draw_pixel(dx + CURSOR_WIDTH - 1, dy + 1, pixel);
    draw_pixel(dx + CURSOR_WIDTH - 1, dy + 2, pixel);

    draw_pixel(dx + 0, dy + CURSOR_HEIGHT - 1, pixel);
    draw_pixel(dx + 1, dy + CURSOR_HEIGHT - 1, pixel);
    draw_pixel(dx + 2, dy + CURSOR_HEIGHT - 1, pixel);
    draw_pixel(dx + 0, dy + CURSOR_HEIGHT - 2, pixel);
    draw_pixel(dx + 0, dy + CURSOR_HEIGHT - 3, pixel);

    draw_pixel(dx + CURSOR_WIDTH - 1, dy + CURSOR_HEIGHT - 1, pixel);
    draw_pixel(dx + CURSOR_WIDTH - 2, dy + CURSOR_HEIGHT - 1, pixel);
    draw_pixel(dx + CURSOR_WIDTH - 3, dy + CURSOR_HEIGHT - 1, pixel);
    draw_pixel(dx + CURSOR_WIDTH - 1, dy + CURSOR_HEIGHT - 2, pixel);
    draw_pixel(dx + CURSOR_WIDTH - 1, dy + CURSOR_HEIGHT - 3, pixel);
}

void cursor_draw(int8_t x, int8_t y)
{
    cursor_draw_erase(x, y, 1);
    display_refresh();
}

void cursor_erase(int8_t x, int8_t y)
{
    cursor_draw_erase(x, y, 0);
}

static bool char_shift_left(uint8_t pos_x, uint8_t pos_y, uint8_t str_len)
{
    uint8_t shift_bit = str_len * (CHARACTER_WIDTH + 1);

    uint32_t *addr_start = (uint32_t *)&_display_buffer[(pos_x / 32) * 4 + pos_y * 16];
    uint32_t *addr_end = (uint32_t *)&_display_buffer[12 + pos_y * 16];

    if (shift_bit < 32 && shift_bit > 0 && addr_start < addr_end)
    {
        for (uint32_t y = 0; y < CHARACTER_HEIGHT; y++)
        {
            uint32_t mask = (1 << (pos_x % 32)) - 1;
            uint32_t frist = *addr_start & mask;

            uint32_t temp, carry = 0;

            for (uint32_t *addr = addr_start; addr < addr_end; addr++)
            {
                temp = *addr >> (32 - shift_bit);
                *addr = (*addr << shift_bit) | carry;
                carry = temp;
            }

            *addr_start = (*addr_start & ~(mask << shift_bit)) | frist;

            addr_start += 4;
            addr_end += 4;
        }
        return true;
    }
    else
        return false;
}

static bool char_shift_right(uint8_t pos_x, uint8_t pos_y, uint8_t str_len)
{
    uint8_t shift_bit = str_len * (CHARACTER_WIDTH + 1);

    uint32_t *addr_start = (uint32_t *)&_display_buffer[(pos_x / 32) * 4 + pos_y * 16];
    uint32_t *addr_end = (uint32_t *)&_display_buffer[12 + pos_y * 16];

    if (shift_bit < 32 && shift_bit > 0 && addr_start < addr_end)
    {
        for (uint32_t y = 0; y < CHARACTER_HEIGHT; y++)
        {
            uint32_t mask = (1 << (pos_x % 32)) - 1;
            uint32_t frist = *addr_start & mask;

            uint32_t temp, carry = 0;

            for (uint32_t *addr = addr_end - 1; addr >= addr_start; addr--)
            {
                temp = *addr << (32 - shift_bit);
                *addr = (*addr >> shift_bit) | carry;
                carry = temp;
            }

            *addr_start = (*addr_start & ~mask) | frist;

            addr_start += 4;
            addr_end += 4;
        }
        return true;
    }
    else
        return false;
}

static uint8_t char_shift_left_draw(uint8_t pos_x, uint8_t pos_y, const char *str)
{
    uint8_t str_len = strlen(str);

    if (str_len != 0 && char_shift_left(pos_x, pos_y, str_len))
        draw_string(pos_x + str_len * (CHARACTER_WIDTH + 1) + 1, pos_y, str);

    return str_len;
}

void operand_draw_ps(const char *prefix, const char *suffix)
{
    uint8_t prefix_len = char_shift_left_draw(OPERAND_OFFSET_X + cur_opnd_length * (CHARACTER_WIDTH + 1), OPERAND_OFFSET_Y, prefix);
    uint8_t suffix_len = char_shift_left_draw(OPERAND_OFFSET_X, OPERAND_OFFSET_Y, suffix);

    cur_opnd_length += prefix_len + suffix_len;

    display_refresh();
}

void result_draw_ps(const char *prefix, const char *suffix)
{
    uint8_t prefix_len = char_shift_left_draw(RESULT_OFFSET_X + tar_opnd_length * (CHARACTER_WIDTH + 1), RESULT_OFFSET_Y, prefix);
    uint8_t suffix_len = char_shift_left_draw(RESULT_OFFSET_X, RESULT_OFFSET_Y, suffix);

    tar_opnd_length += prefix_len + suffix_len;

    display_refresh();
}

void operand_erase_ps(uint8_t prefix_len, uint8_t suffix_len)
{
    char_shift_right(OPERAND_OFFSET_X + (cur_opnd_length - prefix_len) * (CHARACTER_WIDTH + 1), OPERAND_OFFSET_Y, prefix_len);
    char_shift_right(OPERAND_OFFSET_X, OPERAND_OFFSET_Y, suffix_len);

    cur_opnd_length -= prefix_len + suffix_len;

    display_refresh();
}
void result_erase_ps(uint8_t prefix_len, uint8_t suffix_len)
{
    char_shift_right(RESULT_OFFSET_X + (tar_opnd_length - prefix_len) * (CHARACTER_WIDTH + 1), RESULT_OFFSET_Y, prefix_len);
    char_shift_right(RESULT_OFFSET_X, RESULT_OFFSET_Y, suffix_len);

    tar_opnd_length -= prefix_len + suffix_len;

    display_refresh();
}

static void char_clear(uint8_t pos_y)
{
    for (int32_t i = 0; i < CHARACTER_HEIGHT; i++)
    {
        *(uint32_t *)&_display_buffer[((i + pos_y) << 4) + 0] = 0;
        *(uint32_t *)&_display_buffer[((i + pos_y) << 4) + 4] = 0;
        *(uint32_t *)&_display_buffer[((i + pos_y) << 4) + 8] = 0;
    }
}

void operand_cur_draw_clear()
{
    uint8_t step;
    while (cur_opnd_length != 0)
    {
        step = (cur_opnd_length > MAX_SHIFT_LENGTH) ? MAX_SHIFT_LENGTH : cur_opnd_length;
        char_shift_right(OPERAND_OFFSET_X, OPERAND_OFFSET_Y, step);
        cur_opnd_length -= step;
    }
}

void operand_draw_clear()
{
    char_clear(OPERAND_OFFSET_Y);
    cur_opnd_length = 0;
}

void result_draw_clear()
{
    char_clear(RESULT_OFFSET_Y);
    tar_opnd_length = 0;
}

void operand_draw_full(double result)
{
    char str[MAX_DISPLAY_LENGTH + 1];
    uint8_t str_len = snprintf(str, sizeof(str), "%g", result);
    uint8_t delta, step;

    while (cur_opnd_length != str_len)
    {
        if (cur_opnd_length > str_len)
        {
            delta = cur_opnd_length - str_len;
            step = (delta > MAX_SHIFT_LENGTH) ? MAX_SHIFT_LENGTH : delta;
            char_shift_right(OPERAND_OFFSET_X, OPERAND_OFFSET_Y, step);
            cur_opnd_length -= step;
        }
        else
        {
            delta = str_len - cur_opnd_length;
            step = (delta > MAX_SHIFT_LENGTH) ? MAX_SHIFT_LENGTH : delta;
            char_shift_left(OPERAND_OFFSET_X, OPERAND_OFFSET_Y, step);
            cur_opnd_length += step;
        }
    }

    draw_string(OPERAND_OFFSET_X + cur_opnd_length * (CHARACTER_WIDTH + 1) + 1, OPERAND_OFFSET_Y, str);
    display_refresh();
}

void result_draw_full(double result)
{
    result_draw_clear();
    char str[MAX_DISPLAY_LENGTH + 1];
    tar_opnd_length = snprintf(str, sizeof(str), "%g", result);
    draw_string(RESULT_OFFSET_X + tar_opnd_length * (CHARACTER_WIDTH + 1) + 1, RESULT_OFFSET_Y, str);
    display_refresh();
}

void error_draw(const char *str)
{
    result_draw_clear();
    tar_opnd_length = strlen(str);
    draw_string(RESULT_OFFSET_X + tar_opnd_length * (CHARACTER_WIDTH + 1) + 1, RESULT_OFFSET_Y, str);
    display_refresh();
}

void draw_init()
{
    for (int8_t x = 0; x < KEY_NUMBER_X; x++)
        for (int8_t y = 0; y < KEY_NUMBER_Y; y++)
            draw_grid_short(KEY_OFFSET_X + x * KEY_STEP_X,
                            KEY_OFFSET_Y + y * KEY_STEP_Y,
                            KEY_WIDTH, KEY_HEIGHT,
                            key_grid[key_pos[x][y]]);
    cursor_draw(0, 0);
    display_refresh();
}
