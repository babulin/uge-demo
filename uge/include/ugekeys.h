#pragma once


 //
 // UGE 输入事件结构体
 //
struct ugeInputEvent {
    ugeInputEvent(int type, int chr) : type(type), chr(chr) {}

    int type;       // 事件类型
    int key{};      // key码
    int flags{};    // 事件标识
    int chr;        // 字符码
    int wheel{};    // 滚轮
    float x{};      // 鼠标指针x坐标
    float y{};      // 鼠标指针y坐标
};


//
// UGE Input Event type constants
//
enum {
    INPUT_KEYDOWN = 1,
    INPUT_KEYUP = 2,
    INPUT_MBUTTONDOWN = 3,
    INPUT_MBUTTONUP = 4,
    INPUT_MOUSEMOVE = 5,
    INPUT_MOUSEWHEEL = 6,
};


//
// UGE Input Event flags
//
enum {
    UGEINP_SHIFT = 1,
    UGEINP_CTRL = 2,
    UGEINP_ALT = 4,
    UGEINP_CAPSLOCK = 8,
    UGEINP_SCROLLLOCK = 16,
    UGEINP_NUMLOCK = 32,
    UGEINP_REPEAT = 64,
};

//
// UGE Virtual-key codes
//
typedef enum {
    UGEK_NO_KEY = 0x00,
    UGEK_LBUTTON = 0x01,
    UGEK_RBUTTON = 0x02,
    UGEK_MBUTTON = 0x04,
    UGEK_ESCAPE = 0x1B,
    UGEK_BACKSPACE = 0x08,
    UGEK_TAB = 0x09,
    UGEK_ENTER = 0x0D,
    UGEK_SPACE = 0x20,
    UGEK_SHIFT = 0x10,
    UGEK_CTRL = 0x11,
    UGEK_ALT = 0x12,
    UGEK_LWIN = 0x5B,
    UGEK_RWIN = 0x5C,
    UGEK_APPS = 0x5D,
    UGEK_PAUSE = 0x13,
    UGEK_CAPSLOCK = 0x14,
    UGEK_NUMLOCK = 0x90,
    UGEK_SCROLLLOCK = 0x91,
    UGEK_PGUP = 0x21,
    UGEK_PGDN = 0x22,
    UGEK_HOME = 0x24,
    UGEK_END = 0x23,
    UGEK_INSERT = 0x2D,
    UGEK_DELETE = 0x2E,
    UGEK_LEFT = 0x25,
    UGEK_UP = 0x26,
    UGEK_RIGHT = 0x27,
    UGEK_DOWN = 0x28,
    UGEK_0 = 0x30,
    UGEK_1 = 0x31,
    UGEK_2 = 0x32,
    UGEK_3 = 0x33,
    UGEK_4 = 0x34,
    UGEK_5 = 0x35,
    UGEK_6 = 0x36,
    UGEK_7 = 0x37,
    UGEK_8 = 0x38,
    UGEK_9 = 0x39,
    UGEK_A = 0x41,
    UGEK_B = 0x42,
    UGEK_C = 0x43,
    UGEK_D = 0x44,
    UGEK_E = 0x45,
    UGEK_F = 0x46,
    UGEK_G = 0x47,
    UGEK_H = 0x48,
    UGEK_I = 0x49,
    UGEK_J = 0x4A,
    UGEK_K = 0x4B,
    UGEK_L = 0x4C,
    UGEK_M = 0x4D,
    UGEK_N = 0x4E,
    UGEK_O = 0x4F,
    UGEK_P = 0x50,
    UGEK_Q = 0x51,
    UGEK_R = 0x52,
    UGEK_S = 0x53,
    UGEK_T = 0x54,
    UGEK_U = 0x55,
    UGEK_V = 0x56,
    UGEK_W = 0x57,
    UGEK_X = 0x58,
    UGEK_Y = 0x59,
    UGEK_Z = 0x5A,
    UGEK_GRAVE = 0xC0,
    UGEK_MINUS = 0xBD,
    UGEK_EQUALS = 0xBB,
    UGEK_BACKSLASH = 0xDC,
    UGEK_LBRACKET = 0xDB,
    UGEK_RBRACKET = 0xDD,
    UGEK_SEMICOLON = 0xBA,
    UGEK_APOSTROPHE = 0xDE,
    UGEK_COMMA = 0xBC,
    UGEK_PERIOD = 0xBE,
    UGEK_SLASH = 0xBF,
    UGEK_NUMPAD0 = 0x60,
    UGEK_NUMPAD1 = 0x61,
    UGEK_NUMPAD2 = 0x62,
    UGEK_NUMPAD3 = 0x63,
    UGEK_NUMPAD4 = 0x64,
    UGEK_NUMPAD5 = 0x65,
    UGEK_NUMPAD6 = 0x66,
    UGEK_NUMPAD7 = 0x67,
    UGEK_NUMPAD8 = 0x68,
    UGEK_NUMPAD9 = 0x69,
    UGEK_MULTIPLY = 0x6A,
    UGEK_DIVIDE = 0x6F,
    UGEK_ADD = 0x6B,
    UGEK_SUBTRACT = 0x6D,
    UGEK_DECIMAL = 0x6E,
    UGEK_F1 = 0x70,
    UGEK_F2 = 0x71,
    UGEK_F3 = 0x72,
    UGEK_F4 = 0x73,
    UGEK_F5 = 0x74,
    UGEK_F6 = 0x75,
    UGEK_F7 = 0x76,
    UGEK_F8 = 0x77,
    UGEK_F9 = 0x78,
    UGEK_F10 = 0x79,
    UGEK_F11 = 0x7A,
    UGEK_F12 = 0x7B
} UGEKeyCode_t;
