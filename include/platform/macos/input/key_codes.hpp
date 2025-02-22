#ifndef INTERVIEW_CHEATER_PLATFORM_MACOS_KEY_CODES_HPP
#define INTERVIEW_CHEATER_PLATFORM_MACOS_KEY_CODES_HPP

#include <Carbon/Carbon.h>

/**
 * @brief Virtual key codes for keyboard input.
 *
 * This namespace contains cross-platform virtual key codes for both
 * regular keys and modifier keys. These codes are used to identify
 * specific keys and modifiers in the input system.
 *
 * The key codes are mapped to macOS Carbon key codes and
 * are guaranteed to be compatible with Windows equivalents.
 */
namespace interview_cheater::input::KeyCodes {

// Letters (common between Windows and macOS)
constexpr int KEY_A = kVK_ANSI_A;  ///< Virtual key code for 'A'
constexpr int KEY_B = kVK_ANSI_B;  ///< Virtual key code for 'B'
constexpr int KEY_C = kVK_ANSI_C;  ///< Virtual key code for 'C'
constexpr int KEY_D = kVK_ANSI_D;  ///< Virtual key code for 'D'
constexpr int KEY_E = kVK_ANSI_E;  ///< Virtual key code for 'E'
constexpr int KEY_F = kVK_ANSI_F;  ///< Virtual key code for 'F'
constexpr int KEY_G = kVK_ANSI_G;  ///< Virtual key code for 'G'
constexpr int KEY_H = kVK_ANSI_H;  ///< Virtual key code for 'H'
constexpr int KEY_I = kVK_ANSI_I;  ///< Virtual key code for 'I'
constexpr int KEY_J = kVK_ANSI_J;  ///< Virtual key code for 'J'
constexpr int KEY_K = kVK_ANSI_K;  ///< Virtual key code for 'K'
constexpr int KEY_L = kVK_ANSI_L;  ///< Virtual key code for 'L'
constexpr int KEY_M = kVK_ANSI_M;  ///< Virtual key code for 'M'
constexpr int KEY_N = kVK_ANSI_N;  ///< Virtual key code for 'N'
constexpr int KEY_O = kVK_ANSI_O;  ///< Virtual key code for 'O'
constexpr int KEY_P = kVK_ANSI_P;  ///< Virtual key code for 'P'
constexpr int KEY_Q = kVK_ANSI_Q;  ///< Virtual key code for 'Q'
constexpr int KEY_R = kVK_ANSI_R;  ///< Virtual key code for 'R'
constexpr int KEY_S = kVK_ANSI_S;  ///< Virtual key code for 'S'
constexpr int KEY_T = kVK_ANSI_T;  ///< Virtual key code for 'T'
constexpr int KEY_U = kVK_ANSI_U;  ///< Virtual key code for 'U'
constexpr int KEY_V = kVK_ANSI_V;  ///< Virtual key code for 'V'
constexpr int KEY_W = kVK_ANSI_W;  ///< Virtual key code for 'W'
constexpr int KEY_X = kVK_ANSI_X;  ///< Virtual key code for 'X'
constexpr int KEY_Y = kVK_ANSI_Y;  ///< Virtual key code for 'Y'
constexpr int KEY_Z = kVK_ANSI_Z;  ///< Virtual key code for 'Z'

// Numbers (common between Windows and macOS)
constexpr int KEY_0 = kVK_ANSI_0;  ///< Virtual key code for '0'
constexpr int KEY_1 = kVK_ANSI_1;  ///< Virtual key code for '1'
constexpr int KEY_2 = kVK_ANSI_2;  ///< Virtual key code for '2'
constexpr int KEY_3 = kVK_ANSI_3;  ///< Virtual key code for '3'
constexpr int KEY_4 = kVK_ANSI_4;  ///< Virtual key code for '4'
constexpr int KEY_5 = kVK_ANSI_5;  ///< Virtual key code for '5'
constexpr int KEY_6 = kVK_ANSI_6;  ///< Virtual key code for '6'
constexpr int KEY_7 = kVK_ANSI_7;  ///< Virtual key code for '7'
constexpr int KEY_8 = kVK_ANSI_8;  ///< Virtual key code for '8'
constexpr int KEY_9 = kVK_ANSI_9;  ///< Virtual key code for '9'

// Function keys (common between Windows and macOS)
constexpr int KEY_F1 = kVK_F1;    ///< Virtual key code for F1
constexpr int KEY_F2 = kVK_F2;    ///< Virtual key code for F2
constexpr int KEY_F3 = kVK_F3;    ///< Virtual key code for F3
constexpr int KEY_F4 = kVK_F4;    ///< Virtual key code for F4
constexpr int KEY_F5 = kVK_F5;    ///< Virtual key code for F5
constexpr int KEY_F6 = kVK_F6;    ///< Virtual key code for F6
constexpr int KEY_F7 = kVK_F7;    ///< Virtual key code for F7
constexpr int KEY_F8 = kVK_F8;    ///< Virtual key code for F8
constexpr int KEY_F9 = kVK_F9;    ///< Virtual key code for F9
constexpr int KEY_F10 = kVK_F10;  ///< Virtual key code for F10
constexpr int KEY_F11 = kVK_F11;  ///< Virtual key code for F11
constexpr int KEY_F12 = kVK_F12;  ///< Virtual key code for F12
constexpr int KEY_F13 = kVK_F13;  ///< Virtual key code for F13
constexpr int KEY_F14 = kVK_F14;  ///< Virtual key code for F14
constexpr int KEY_F15 = kVK_F15;  ///< Virtual key code for F15
constexpr int KEY_F16 = kVK_F16;  ///< Virtual key code for F16
constexpr int KEY_F17 = kVK_F17;  ///< Virtual key code for F17
constexpr int KEY_F18 = kVK_F18;  ///< Virtual key code for F18
constexpr int KEY_F19 = kVK_F19;  ///< Virtual key code for F19

// Special keys (common between Windows and macOS)
constexpr int KEY_ESCAPE = kVK_Escape;         ///< Virtual key code for Escape
constexpr int KEY_TAB = kVK_Tab;               ///< Virtual key code for Tab
constexpr int KEY_CAPSLOCK = kVK_CapsLock;     ///< Virtual key code for Caps Lock
constexpr int KEY_SPACE = kVK_Space;           ///< Virtual key code for Space
constexpr int KEY_BACKSPACE = kVK_Delete;      ///< Virtual key code for Backspace
constexpr int KEY_ENTER = kVK_Return;          ///< Virtual key code for Enter
constexpr int KEY_DELETE = kVK_ForwardDelete;  ///< Virtual key code for Delete
constexpr int KEY_HOME = kVK_Home;             ///< Virtual key code for Home
constexpr int KEY_END = kVK_End;               ///< Virtual key code for End
constexpr int KEY_PAGEUP = kVK_PageUp;         ///< Virtual key code for Page Up
constexpr int KEY_PAGEDOWN = kVK_PageDown;     ///< Virtual key code for Page Down

// Arrow keys (common between Windows and macOS)
constexpr int KEY_UP = kVK_UpArrow;        ///< Virtual key code for Up Arrow
constexpr int KEY_DOWN = kVK_DownArrow;    ///< Virtual key code for Down Arrow
constexpr int KEY_LEFT = kVK_LeftArrow;    ///< Virtual key code for Left Arrow
constexpr int KEY_RIGHT = kVK_RightArrow;  ///< Virtual key code for Right Arrow

// Modifier keys (common between Windows and macOS)
constexpr int CONTROL_MODIFIER = kVK_Control;  ///< Virtual key code for Control
constexpr int ALT_MODIFIER = kVK_Option;       ///< Virtual key code for Alt/Option
constexpr int SHIFT_MODIFIER = kVK_Shift;      ///< Virtual key code for Shift
                                                // TODO(dev): Check why left CMD triggers both left and right CMD keys
constexpr int COMMAND_MODIFIER = kVK_Control;  ///< Virtual key code for Command/Windows key

// Special masks (common between Windows and macOS)
constexpr uint16_t KEY_PRESSED_MASK = 0x0100U;  ///< Mask for checking key pressed state

// Additional keys (common between Windows and macOS)
constexpr int KEY_SLASH = kVK_ANSI_Slash;  ///< Virtual key code for forward slash

}  // namespace interview_cheater::input::KeyCodes

#endif  // INTERVIEW_CHEATER_PLATFORM_MACOS_KEY_CODES_HPP