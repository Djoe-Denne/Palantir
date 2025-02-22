#ifndef INTERVIEW_CHEATER_KEY_CODES_HPP
#define INTERVIEW_CHEATER_KEY_CODES_HPP

#include <windows.h>
/**
 * @brief Virtual key codes for keyboard input.
 *
 * This namespace contains cross-platform virtual key codes for both
 * regular keys and modifier keys. These codes are used to identify
 * specific keys and modifiers in the input system.
 *
 * The key codes are mapped to Windows Virtual Key Codes (VK_*) and
 * are guaranteed to be compatible with macOS equivalents.
 */
namespace interview_cheater::input::KeyCodes {

// Letters (common between Windows and macOS)
constexpr int KEY_A = 'A';  ///< Virtual key code for 'A'
constexpr int KEY_B = 'B';  ///< Virtual key code for 'B'
constexpr int KEY_C = 'C';  ///< Virtual key code for 'C'
constexpr int KEY_D = 'D';  ///< Virtual key code for 'D'
constexpr int KEY_E = 'E';  ///< Virtual key code for 'E'
constexpr int KEY_F = 'F';  ///< Virtual key code for 'F'
constexpr int KEY_G = 'G';  ///< Virtual key code for 'G'
constexpr int KEY_H = 'H';  ///< Virtual key code for 'H'
constexpr int KEY_I = 'I';  ///< Virtual key code for 'I'
constexpr int KEY_J = 'J';  ///< Virtual key code for 'J'
constexpr int KEY_K = 'K';  ///< Virtual key code for 'K'
constexpr int KEY_L = 'L';  ///< Virtual key code for 'L'
constexpr int KEY_M = 'M';  ///< Virtual key code for 'M'
constexpr int KEY_N = 'N';  ///< Virtual key code for 'N'
constexpr int KEY_O = 'O';  ///< Virtual key code for 'O'
constexpr int KEY_P = 'P';  ///< Virtual key code for 'P'
constexpr int KEY_Q = 'Q';  ///< Virtual key code for 'Q'
constexpr int KEY_R = 'R';  ///< Virtual key code for 'R'
constexpr int KEY_S = 'S';  ///< Virtual key code for 'S'
constexpr int KEY_T = 'T';  ///< Virtual key code for 'T'
constexpr int KEY_U = 'U';  ///< Virtual key code for 'U'
constexpr int KEY_V = 'V';  ///< Virtual key code for 'V'
constexpr int KEY_W = 'W';  ///< Virtual key code for 'W'
constexpr int KEY_X = 'X';  ///< Virtual key code for 'X'
constexpr int KEY_Y = 'Y';  ///< Virtual key code for 'Y'
constexpr int KEY_Z = 'Z';  ///< Virtual key code for 'Z'

// Numbers (common between Windows and macOS)
constexpr int KEY_0 = '0';  ///< Virtual key code for '0'
constexpr int KEY_1 = '1';  ///< Virtual key code for '1'
constexpr int KEY_2 = '2';  ///< Virtual key code for '2'
constexpr int KEY_3 = '3';  ///< Virtual key code for '3'
constexpr int KEY_4 = '4';  ///< Virtual key code for '4'
constexpr int KEY_5 = '5';  ///< Virtual key code for '5'
constexpr int KEY_6 = '6';  ///< Virtual key code for '6'
constexpr int KEY_7 = '7';  ///< Virtual key code for '7'
constexpr int KEY_8 = '8';  ///< Virtual key code for '8'
constexpr int KEY_9 = '9';  ///< Virtual key code for '9'

// Function keys (common between Windows and macOS)
constexpr int KEY_F1 = VK_F1;    ///< Virtual key code for F1
constexpr int KEY_F2 = VK_F2;    ///< Virtual key code for F2
constexpr int KEY_F3 = VK_F3;    ///< Virtual key code for F3
constexpr int KEY_F4 = VK_F4;    ///< Virtual key code for F4
constexpr int KEY_F5 = VK_F5;    ///< Virtual key code for F5
constexpr int KEY_F6 = VK_F6;    ///< Virtual key code for F6
constexpr int KEY_F7 = VK_F7;    ///< Virtual key code for F7
constexpr int KEY_F8 = VK_F8;    ///< Virtual key code for F8
constexpr int KEY_F9 = VK_F9;    ///< Virtual key code for F9
constexpr int KEY_F10 = VK_F10;  ///< Virtual key code for F10
constexpr int KEY_F11 = VK_F11;  ///< Virtual key code for F11
constexpr int KEY_F12 = VK_F12;  ///< Virtual key code for F12
constexpr int KEY_F13 = VK_F13;  ///< Virtual key code for F13
constexpr int KEY_F14 = VK_F14;  ///< Virtual key code for F14
constexpr int KEY_F15 = VK_F15;  ///< Virtual key code for F15
constexpr int KEY_F16 = VK_F16;  ///< Virtual key code for F16
constexpr int KEY_F17 = VK_F17;  ///< Virtual key code for F17
constexpr int KEY_F18 = VK_F18;  ///< Virtual key code for F18
constexpr int KEY_F19 = VK_F19;  ///< Virtual key code for F19

// Special keys (common between Windows and macOS)
constexpr int KEY_ESCAPE = VK_ESCAPE;     ///< Virtual key code for Escape
constexpr int KEY_TAB = VK_TAB;           ///< Virtual key code for Tab
constexpr int KEY_CAPSLOCK = VK_CAPITAL;  ///< Virtual key code for Caps Lock
constexpr int KEY_SPACE = VK_SPACE;       ///< Virtual key code for Space
constexpr int KEY_BACKSPACE = VK_BACK;    ///< Virtual key code for Backspace
constexpr int KEY_ENTER = VK_RETURN;      ///< Virtual key code for Enter
constexpr int KEY_DELETE = VK_DELETE;     ///< Virtual key code for Delete
constexpr int KEY_HOME = VK_HOME;         ///< Virtual key code for Home
constexpr int KEY_END = VK_END;           ///< Virtual key code for End
constexpr int KEY_PAGEUP = VK_PRIOR;      ///< Virtual key code for Page Up
constexpr int KEY_PAGEDOWN = VK_NEXT;     ///< Virtual key code for Page Down

// Arrow keys (common between Windows and macOS)
constexpr int KEY_UP = VK_UP;        ///< Virtual key code for Up Arrow
constexpr int KEY_DOWN = VK_DOWN;    ///< Virtual key code for Down Arrow
constexpr int KEY_LEFT = VK_LEFT;    ///< Virtual key code for Left Arrow
constexpr int KEY_RIGHT = VK_RIGHT;  ///< Virtual key code for Right Arrow

// Modifier keys (common between Windows and macOS)
constexpr int CONTROL_MODIFIER = VK_CONTROL;  ///< Virtual key code for Control
constexpr int ALT_MODIFIER = VK_MENU;         ///< Virtual key code for Alt/Option
constexpr int SHIFT_MODIFIER = VK_SHIFT;      ///< Virtual key code for Shift
constexpr int COMMAND_MODIFIER = VK_LWIN;     ///< Virtual key code for Command/Windows key

// Special masks (common between Windows and macOS)
constexpr uint16_t KEY_PRESSED_MASK = 0x8000U;  ///< Mask for checking key pressed state

// Additional keys (common between Windows and macOS)
constexpr int KEY_SLASH = VK_OEM_2;  ///< Virtual key code for forward slash

}  // namespace interview_cheater::input::KeyCodes

#endif  // INTERVIEW_CHEATER_KEY_CODES_HPP