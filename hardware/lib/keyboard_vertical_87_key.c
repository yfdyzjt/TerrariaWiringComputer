#include "keyboard_vertical_87_key.h"

#define K_00 0x41
#define K_01 0x61
#define K_02 0x71
#define K_03 0x79
#define K_04 0x39
#define K_05 0x59
#define K_06 0x29
#define K_07 0x51
#define K_08 0x69
#define K_09 0x31
#define K_10 0x19
#define K_11 0x49
#define K_12 0x21
#define K_13 0x11
#define K_14 0x09
#define K_15 0x01
#define K_16 0x42
#define K_17 0x62
#define K_18 0x72
#define K_19 0x7a
#define K_20 0x3a
#define K_21 0x5a
#define K_22 0x2a
#define K_23 0x52
#define K_24 0x6a
#define K_25 0x32
#define K_26 0x1a
#define K_27 0x4a
#define K_28 0x22
#define K_29 0x12
#define K_30 0x0a
#define K_31 0x02
#define K_32 0x45
#define K_33 0x65
#define K_34 0x75
#define K_35 0x7d
#define K_36 0x3d
#define K_37 0x5d
#define K_38 0x2d
#define K_39 0x55
#define K_40 0x6d
#define K_41 0x35
#define K_42 0x1d
#define K_43 0x4d
#define K_44 0x25
#define K_45 0x15
#define K_46 0x0d
#define K_47 0x05
#define K_48 0x43
#define K_49 0x63
#define K_50 0x73
#define K_51 0x7b
#define K_52 0x3b
#define K_53 0x5b
#define K_54 0x2b
#define K_55 0x53
#define K_56 0x6b
#define K_57 0x33
#define K_58 0x1b
#define K_59 0x4b
#define K_60 0x23
#define K_61 0x13
#define K_62 0x0b
#define K_63 0x03
#define K_64 0x46
#define K_65 0x66
#define K_66 0x76
#define K_67 0x7e
#define K_68 0x3e
#define K_69 0x5e
#define K_70 0x2e
#define K_71 0x56
#define K_72 0x6e
#define K_73 0x36
#define K_74 0x1e
#define K_75 0x4e
#define K_76 0x26
#define K_77 0x16
#define K_78 0x0e
#define K_79 0x06
#define K_80 0x44
#define K_81 0x64
#define K_82 0x74
#define K_83 0x7c
#define K_84 0x3c
#define K_85 0x5c
#define K_86 0x2c
#define K_87 0x54
#define K_88 0x6c
#define K_89 0x34
#define K_90 0x1c
#define K_91 0x4c
#define K_92 0x24
#define K_93 0x14
#define K_94 0x0c
#define K_95 0x04

volatile const unsigned char _keyboard_queue __attribute__((section("keyboard_vertical_87_key.h")));
volatile unsigned char *_keyboard_reset = (volatile unsigned char *)&_keyboard_queue;

static const char kc_to_vkc[128] = {
    /* Line 1 */
    [K_00] = VK_ESCAPE,
    [K_01] = 0,
    [K_02] = VK_F1,
    [K_03] = VK_F2,
    [K_04] = VK_F3,
    [K_05] = VK_F4,
    [K_06] = VK_F5,
    [K_07] = VK_F6,
    [K_08] = VK_F7,
    [K_09] = VK_F8,
    [K_10] = VK_F9,
    [K_11] = VK_F10,
    [K_12] = VK_F11,
    [K_13] = VK_F12,
    [K_14] = VK_SNAPSHOT,
    [K_15] = VK_NUMLOCK,

    /* Line 2 */
    [K_16] = VK_OEM_3,
    [K_17] = VK_KEY_1,
    [K_18] = VK_KEY_2,
    [K_19] = VK_KEY_3,
    [K_20] = VK_KEY_4,
    [K_21] = VK_KEY_5,
    [K_22] = VK_KEY_6,
    [K_23] = VK_KEY_7,
    [K_24] = VK_KEY_8,
    [K_25] = VK_KEY_9,
    [K_26] = VK_KEY_0,
    [K_27] = VK_OEM_MINUS,
    [K_28] = VK_OEM_PLUS,
    [K_29] = VK_BACK,
    [K_30] = VK_INSERT,
    [K_31] = VK_HOME,

    /* Line 3 */
    [K_32] = VK_TAB,
    [K_33] = VK_KEY_Q,
    [K_34] = VK_KEY_W,
    [K_35] = VK_KEY_E,
    [K_36] = VK_KEY_R,
    [K_37] = VK_KEY_T,
    [K_38] = VK_KEY_Y,
    [K_39] = VK_KEY_U,
    [K_40] = VK_KEY_I,
    [K_41] = VK_KEY_O,
    [K_42] = VK_KEY_P,
    [K_43] = VK_OEM_4,
    [K_44] = VK_OEM_6,
    [K_45] = VK_OEM_5,
    [K_46] = VK_DELETE,
    [K_47] = VK_END,

    /* Line 4 */
    [K_48] = VK_CAPITAL,
    [K_49] = VK_KEY_A,
    [K_50] = VK_KEY_S,
    [K_51] = VK_KEY_D,
    [K_52] = VK_KEY_F,
    [K_53] = VK_KEY_G,
    [K_54] = VK_KEY_H,
    [K_55] = VK_KEY_J,
    [K_56] = VK_KEY_K,
    [K_57] = VK_KEY_L,
    [K_58] = VK_OEM_1,
    [K_59] = VK_OEM_7,
    [K_60] = VK_RETURN,
    [K_61] = 0,
    [K_62] = 0,
    [K_63] = VK_NEXT,

    /* Line 5 */
    [K_64] = VK_LSHIFT,
    [K_65] = VK_KEY_Z,
    [K_66] = VK_KEY_X,
    [K_67] = VK_KEY_C,
    [K_68] = VK_KEY_V,
    [K_69] = VK_KEY_B,
    [K_70] = VK_KEY_N,
    [K_71] = VK_KEY_M,
    [K_72] = VK_OEM_COMMA,
    [K_73] = VK_OEM_PERIOD,
    [K_74] = VK_OEM_2,
    [K_75] = VK_RSHIFT,
    [K_76] = 0,
    [K_77] = VK_UP,
    [K_78] = 0,
    [K_79] = VK_PRIOR,

    /* Line 6 */
    [K_80] = VK_LCONTROL,
    [K_81] = VK_LWIN,
    [K_82] = VK_LMENU,
    [K_83] = VK_SPACE,
    [K_84] = 0,
    [K_85] = 0,
    [K_86] = 0,
    [K_87] = 0,
    [K_88] = VK_RMENU,
    [K_89] = VK_RWIN,
    [K_90] = VK_MENU,
    [K_91] = VK_RCONTROL,
    [K_92] = VK_LEFT,
    [K_93] = VK_DOWN,
    [K_94] = VK_RIGHT,
    [K_95] = VK_PAUSE,
};

void keyboard_init()
{
    *_keyboard_reset = 1;
}

unsigned char keyboard_scan()
{
    return kc_to_vkc[_keyboard_queue];
}
