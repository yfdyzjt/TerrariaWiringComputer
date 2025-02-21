#include "key.h"
#include "error.h"
#include "data.h"
#include "output.h"

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

static ErrorType key_number_press(KeyType key);
static ErrorType key_dot_press(KeyType key);
static ErrorType key_neg_press(KeyType key);

static ErrorType key_unary_op_press(KeyType key);
static ErrorType key_binary_op_press(KeyType key);

static ErrorType key_eq_press(KeyType key);
static ErrorType key_back_press(KeyType key);
static ErrorType key_ce_press(KeyType key);
static ErrorType key_c_press(KeyType key);

static ErrorType key_mr_press(KeyType key);
static ErrorType key_mc_press(KeyType key);
static ErrorType key_ms_press(KeyType key);
static ErrorType key_madd_press(KeyType key);
static ErrorType key_msub_press(KeyType key);

static ErrorType key_no_press(KeyType key);

static void overwrite_number_clear()
{
    *cur_opnd = 0;
    allow_number = true;
    allow_overwrite = false;
    result_draw_clear();
}

static void overwrite_op_clear()
{
    allow_overwrite = false;
    operand_draw_full(*cur_opnd);
}

static void equal_clear()
{
    is_equal = false;
    operand_draw_clear();
}

static ErrorType key_no_press(KeyType key)
{
    return ERROR_NO_KEY;
}

static ErrorType key_number_press(KeyType key)
{
    if (!is_error && stack_push(key))
    {
        if (is_equal)
            equal_clear();
        if (allow_overwrite)
            overwrite_number_clear();
        if (allow_number && !(*cur_opnd == 0 && key == KEY_0))
        {
            if (is_dot)
            {
                dot_exp *= 10;
                *cur_opnd += (double)key / dot_exp;
            }
            else
            {
                *cur_opnd *= 10;
                *cur_opnd += (double)key;
            }

            char suffix[] = {'0' + (char)key, '\0'};
            operand_draw_ps("", suffix);
            result_draw_ps("", suffix);
        }
    }

    return ERROR_NONE;
}

static ErrorType key_dot_press(KeyType key)
{
    if (!is_error && !is_dot && stack_push(key))
    {
        if (is_equal)
            equal_clear();
        if (allow_overwrite)
            overwrite_number_clear();
        if (allow_number)
        {
            is_dot = true;
            if (*cur_opnd == 0)
            {
                operand_draw_ps("", "0.");
                result_draw_ps("", "0.");
            }
            else
            {
                operand_draw_ps("", ".");
                result_draw_ps("", ".");
            }
        }
    }
    return ERROR_NONE;
}

static ErrorType key_neg_press(KeyType key)
{
    if (!is_error && *cur_opnd != 0)
    {
        if (is_equal)
            equal_clear();
        if (allow_overwrite)
            overwrite_op_clear();

        is_neg = !is_neg;
        *cur_opnd = -*cur_opnd;

        if (allow_number)
        {
            if (is_neg)
            {
                operand_erase_ps(1, 0);
                result_erase_ps(1, 0);
            }
            else
            {
                operand_draw_ps("-", "");
                result_draw_ps("-", "");
            }
        }
        else
        {
            operand_draw_ps("neg(", ")");
            result_draw_full(*cur_opnd);
        }
    }
    return ERROR_NONE;
}

static ErrorType key_unary_op_press(KeyType key)
{
    if (!is_error)
    {
        if (is_equal)
            equal_clear();
        if (allow_overwrite)
            overwrite_op_clear();

        const char *prefix;
        const char *suffix;

        switch (key)
        {
        case KEY_RECIP:
            if (*cur_opnd != 0)
            {
                *cur_opnd = 1 / *cur_opnd;
                prefix = "1/(";
                suffix = ")";
            }
            else
                return ERROR_DIV_ZERO;
            break;
        case KEY_SQ:
            *cur_opnd *= *cur_opnd;
            prefix = "(";
            suffix = ")^2";
            break;
        case KEY_SQRT:
            if (*cur_opnd >= 0)
            {
                *cur_opnd = sqrt(*cur_opnd);
                prefix = "sqrt(";
                suffix = ")";
            }
            else
                return ERROR_SQRT_NEG;
            break;
        default:
            return ERROR_NO_KEY;
            break;
        }

        allow_number = false;

        operand_draw_ps(prefix, suffix);
        result_draw_full(*cur_opnd);
    }
    return ERROR_NONE;
}

static ErrorType key_binary_op_press(KeyType key)
{
    if (!is_error)
    {
        if (is_equal)
            equal_clear();
        if (allow_overwrite)
            overwrite_op_clear();

        if (bin_op == KEY_NONE)
        {
            const char *suffix;

            switch (key)
            {
            case KEY_ADD:
                suffix = "+";
                break;
            case KEY_SUB:
                suffix = "-";
                break;
            case KEY_MUL:
                suffix = "*";
                break;
            case KEY_DIV:
                suffix = "/";
                break;
            default:
                return ERROR_NO_KEY;
                break;
            }

            bin_op = key;

            operand_draw_ps("", suffix);
            cur_opnd_init(&opnd_2, opnd_1);
        }
        else
        {
            ErrorType err = key_eq_press(KEY_EQ);
            if (err != ERROR_NONE)
                return err;
            return key_binary_op_press(key);
        }
    }
    return ERROR_NONE;
}

static ErrorType key_eq_press(KeyType key)
{
    if (!is_error)
    {
        if (is_equal)
            equal_clear();
        if (allow_overwrite)
            overwrite_op_clear();

        double result;
        if (bin_op == KEY_NONE)
        {
            result = opnd_1;
        }
        else
        {
            switch (bin_op)
            {
            case KEY_ADD:
                result = opnd_1 + opnd_2;
                break;
            case KEY_SUB:
                result = opnd_1 - opnd_2;
                break;
            case KEY_MUL:
                result = opnd_1 * opnd_2;
                break;
            case KEY_DIV:
                if (opnd_2 != 0)
                    result = opnd_1 / opnd_2;
                else
                    return ERROR_DIV_ZERO;
                break;
            default:
                return ERROR_NO_KEY;
                break;
            }
        }

        bin_op = KEY_NONE;
        is_equal = true;

        operand_draw_ps("", "=");
        cur_opnd_init(&opnd_1, result);
    }
    else
    {
        return key_c_press(KEY_C);
    }
    return ERROR_NONE;
}

static ErrorType key_back_press(KeyType key)
{
    if (!is_error)
    {
        if (allow_number)
        {
            if (is_equal)
                equal_clear();

            KeyType last_key;
            if (stack_pop(&last_key))
            {
                if (last_key == KEY_DOT)
                {
                    is_dot = false;
                }
                else
                {
                    if (is_dot)
                    {
                        *cur_opnd -= (double)last_key / dot_exp;
                        dot_exp /= 10;
                    }
                    else
                    {
                        *cur_opnd -= (double)last_key;
                        *cur_opnd /= 10;
                    }
                }

                operand_erase_ps(0, 1);
                result_erase_ps(0, 1);

                if (stack_is_empty())
                    cur_opnd_init(cur_opnd, 0);
            }
        }
    }
    else
    {
        return key_c_press(KEY_C);
    }
    return ERROR_NONE;
}

static ErrorType key_c_press(KeyType key)
{
    bin_op = KEY_NONE;
    is_equal = false;
    operand_draw_clear();
    cur_opnd_init(&opnd_1, 0);
    return ERROR_NONE;
}

static ErrorType key_ce_press(KeyType key)
{
    if (!is_error)
    {
        operand_cur_draw_clear();
        cur_opnd_init(cur_opnd, 0);
    }
    else
    {
        return key_c_press(KEY_C);
    }
    return ERROR_NONE;
}

static ErrorType key_mr_press(KeyType key)
{
    if (!is_error && mem_have)
    {
        operand_cur_draw_clear();
        cur_opnd_init(cur_opnd, mem_opnd);
    }
    return ERROR_NONE;
}

static ErrorType key_mc_press(KeyType key)
{
    if (!is_error && mem_have)
    {
        mem_have = false;
        mem_opnd = 0;
    }
    return ERROR_NONE;
}

static ErrorType key_ms_press(KeyType key)
{
    if (!is_error)
    {
        mem_have = true;
        mem_opnd = *cur_opnd;
    }
    return ERROR_NONE;
}

static ErrorType key_madd_press(KeyType key)
{
    if (!is_error)
    {
        mem_have = true;
        mem_opnd += *cur_opnd;
    }
    return ERROR_NONE;
}

static ErrorType key_msub_press(KeyType key)
{
    if (!is_error)
    {
        mem_have = true;
        mem_opnd -= *cur_opnd;
    }
    return ERROR_NONE;
}

const ErrorType (*key_press[KEY_NUMBER])(KeyType) = {
    [KEY_0] = key_number_press,
    [KEY_1] = key_number_press,
    [KEY_2] = key_number_press,
    [KEY_3] = key_number_press,
    [KEY_4] = key_number_press,
    [KEY_5] = key_number_press,
    [KEY_6] = key_number_press,
    [KEY_7] = key_number_press,
    [KEY_8] = key_number_press,
    [KEY_9] = key_number_press,
    [KEY_DOT] = key_dot_press,
    [KEY_NEG] = key_neg_press,

    [KEY_RECIP] = key_unary_op_press,
    [KEY_SQ] = key_unary_op_press,
    [KEY_SQRT] = key_unary_op_press,

    [KEY_ADD] = key_binary_op_press,
    [KEY_SUB] = key_binary_op_press,
    [KEY_MUL] = key_binary_op_press,
    [KEY_DIV] = key_binary_op_press,

    [KEY_EQ] = key_eq_press,
    [KEY_BACK] = key_back_press,
    [KEY_CE] = key_ce_press,
    [KEY_C] = key_c_press,

    [KEY_MR] = key_mr_press,
    [KEY_MC] = key_mc_press,
    [KEY_MS] = key_ms_press,
    [KEY_MADD] = key_madd_press,
    [KEY_MSUB] = key_msub_press,

    [KEY_NONE] = key_no_press,
};
