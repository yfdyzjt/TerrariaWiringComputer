#ifdef TERMINAL

#include "keyboard_input.h"
#include "keyboard_vertical_87_key.h"

#include <stdbool.h>

ModifierKey modifier_key_state;

bool modifier_key_test(ModifierKey key)
{
    return (modifier_key_state & key) != 0;
}

void modifier_key_press(ModifierKey key)
{
    if (modifier_key_test(key))
        modifier_key_state &= ~key;
    else
        modifier_key_state |= key;
}

void modifier_key_set(VK_KEY vk)
{
    switch (vk)
    {
    case VK_LSHIFT:
    case VK_RSHIFT:
        modifier_key_press(MK_SHIFT);
        break;
    case VK_LCONTROL:
    case VK_RCONTROL:
        modifier_key_press(MK_CTRL);
        break;
    case VK_LMENU:
    case VK_RMENU:
        modifier_key_press(MK_ALT);
        break;
    case VK_LWIN:
        modifier_key_press(MK_WIN);
        break;
    case VK_RWIN:
        modifier_key_press(MK_FN);
        break;
    case VK_CAPITAL:
        modifier_key_press(MK_CAPS_LOCK);
        break;
    default:
        break;
    }
}

char character_key_to_ascii(VK_KEY vk)
{
    char c = '\0';

    if (vk >= VK_KEY_A && vk <= VK_KEY_Z)
    {
        if ((modifier_key_test(MK_SHIFT) && !modifier_key_test(MK_CAPS_LOCK)) ||
            (!modifier_key_test(MK_SHIFT) && modifier_key_test(MK_CAPS_LOCK)))
            c = 'A' + (vk - VK_KEY_A);
        else
            c = 'a' + (vk - VK_KEY_A);
    }
    else if (vk >= VK_KEY_0 && vk <= VK_KEY_9)
    {
        if (!modifier_key_test(MK_SHIFT))
            c = '0' + (vk - VK_KEY_0);
        else
            switch (vk)
            {
            case VK_KEY_1:
                c = '!';
                break;
            case VK_KEY_2:
                c = '@';
                break;
            case VK_KEY_3:
                c = '#';
                break;
            case VK_KEY_4:
                c = '$';
                break;
            case VK_KEY_5:
                c = '%';
                break;
            case VK_KEY_6:
                c = '^';
                break;
            case VK_KEY_7:
                c = '&';
                break;
            case VK_KEY_8:
                c = '*';
                break;
            case VK_KEY_9:
                c = '(';
                break;
            case VK_KEY_0:
                c = ')';
                break;
            default:
                break;
            }
    }
    else if ((vk >= VK_OEM_1 && vk <= VK_OEM_3) ||
             (vk >= VK_OEM_4 && vk <= VK_OEM_7))
    {
        if (!modifier_key_test(MK_SHIFT))
            switch (vk)
            {
            case VK_OEM_PLUS:
                c = '=';
                break;
            case VK_OEM_COMMA:
                c = ',';
                break;
            case VK_OEM_MINUS:
                c = '-';
                break;
            case VK_OEM_PERIOD:
                c = '.';
                break;
            case VK_OEM_1:
                c = ';';
                break;
            case VK_OEM_2:
                c = '/';
                break;
            case VK_OEM_3:
                c = '`';
                break;
            case VK_OEM_4:
                c = '[';
                break;
            case VK_OEM_5:
                c = '\\';
                break;
            case VK_OEM_6:
                c = ']';
                break;
            case VK_OEM_7:
                c = '\'';
                break;
            default:
                break;
            }
        else
            switch (vk)
            {
            case VK_OEM_PLUS:
                c = '+';
                break;
            case VK_OEM_COMMA:
                c = '<';
                break;
            case VK_OEM_MINUS:
                c = '_';
                break;
            case VK_OEM_PERIOD:
                c = '>';
                break;
            case VK_OEM_1:
                c = ':';
                break;
            case VK_OEM_2:
                c = '?';
                break;
            case VK_OEM_3:
                c = '~';
                break;
            case VK_OEM_4:
                c = '{';
                break;
            case VK_OEM_5:
                c = '|';
                break;
            case VK_OEM_6:
                c = '}';
                break;
            case VK_OEM_7:
                c = '"';
            default:
                break;
            }
    }
    else if (vk == VK_SPACE)
    {
        c = ' ';
    }
    else if (vk == VK_TAB)
    {
        c = '\t';
    }
    else if (vk == VK_BACK)
    {
        c = '\b';
    }
    else if (vk == VK_RETURN)
    {
        c = '\n';
    }

    return c;
}

bool is_modifier_key(VK_KEY vk)
{
    return (vk >= VK_LSHIFT && vk <= VK_RMENU) ||
           (vk >= VK_LWIN && vk <= VK_RWIN) ||
           (vk == VK_CAPITAL);
}

bool is_character_key(VK_KEY vk)
{
    return (vk >= VK_KEY_A && vk <= VK_KEY_Z) ||
           (vk >= VK_KEY_0 && vk <= VK_KEY_9) ||
           (vk >= VK_OEM_1 && vk <= VK_OEM_3) ||
           (vk >= VK_OEM_4 && vk <= VK_OEM_7) ||
           (vk == VK_SPACE) ||
           (vk == VK_TAB) ||
           (vk == VK_BACK) ||
           (vk == VK_RETURN);
}

void keyboard_input_init()
{
    keyboard_init();
    modifier_key_state = 0;
}

char keyboard_input_scan()
{
    while (true)
    {
        VK_KEY vk = keyboard_scan();
        if (is_character_key(vk))
            return character_key_to_ascii(vk);
        else if (is_modifier_key(vk))
            modifier_key_set(vk);
    }
}

bool keyboard_input_test(ModifierKey mk)
{
    return modifier_key_test(mk);
}

#endif
