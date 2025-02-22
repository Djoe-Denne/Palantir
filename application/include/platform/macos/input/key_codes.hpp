#ifndef INTERVIEW_CHEATER_PLATFORM_MACOS_KEY_CODES_HPP
#define INTERVIEW_CHEATER_PLATFORM_MACOS_KEY_CODES_HPP

#include <Carbon/Carbon.h>

#include "input/key_register.hpp"
#include "utils/string_utils.hpp"
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

// Initialize all key codes using KeyRegister
static const struct KeyCodeInitializer {
    KeyCodeInitializer() {  // NOLINT
        // Letters
        KeyRegister::registerKey(utils::StringUtils::toUpper("A"), kVK_ANSI_A);
        KeyRegister::registerKey(utils::StringUtils::toUpper("B"), kVK_ANSI_B);
        KeyRegister::registerKey(utils::StringUtils::toUpper("C"), kVK_ANSI_C);
        KeyRegister::registerKey(utils::StringUtils::toUpper("D"), kVK_ANSI_D);
        KeyRegister::registerKey(utils::StringUtils::toUpper("E"), kVK_ANSI_E);
        KeyRegister::registerKey(utils::StringUtils::toUpper("F"), kVK_ANSI_F);
        KeyRegister::registerKey(utils::StringUtils::toUpper("G"), kVK_ANSI_G);
        KeyRegister::registerKey(utils::StringUtils::toUpper("H"), kVK_ANSI_H);
        KeyRegister::registerKey(utils::StringUtils::toUpper("I"), kVK_ANSI_I);
        KeyRegister::registerKey(utils::StringUtils::toUpper("J"), kVK_ANSI_J);
        KeyRegister::registerKey(utils::StringUtils::toUpper("K"), kVK_ANSI_K);
        KeyRegister::registerKey(utils::StringUtils::toUpper("L"), kVK_ANSI_L);
        KeyRegister::registerKey(utils::StringUtils::toUpper("M"), kVK_ANSI_M);
        KeyRegister::registerKey(utils::StringUtils::toUpper("N"), kVK_ANSI_N);
        KeyRegister::registerKey(utils::StringUtils::toUpper("O"), kVK_ANSI_O);
        KeyRegister::registerKey(utils::StringUtils::toUpper("P"), kVK_ANSI_P);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Q"), kVK_ANSI_Q);
        KeyRegister::registerKey(utils::StringUtils::toUpper("R"), kVK_ANSI_R);
        KeyRegister::registerKey(utils::StringUtils::toUpper("S"), kVK_ANSI_S);
        KeyRegister::registerKey(utils::StringUtils::toUpper("T"), kVK_ANSI_T);
        KeyRegister::registerKey(utils::StringUtils::toUpper("U"), kVK_ANSI_U);
        KeyRegister::registerKey(utils::StringUtils::toUpper("V"), kVK_ANSI_V);
        KeyRegister::registerKey(utils::StringUtils::toUpper("W"), kVK_ANSI_W);
        KeyRegister::registerKey(utils::StringUtils::toUpper("X"), kVK_ANSI_X);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Y"), kVK_ANSI_Y);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Z"), kVK_ANSI_Z);

        // Numbers
        KeyRegister::registerKey(utils::StringUtils::toUpper("0"), kVK_ANSI_0);
        KeyRegister::registerKey(utils::StringUtils::toUpper("1"), kVK_ANSI_1);
        KeyRegister::registerKey(utils::StringUtils::toUpper("2"), kVK_ANSI_2);
        KeyRegister::registerKey(utils::StringUtils::toUpper("3"), kVK_ANSI_3);
        KeyRegister::registerKey(utils::StringUtils::toUpper("4"), kVK_ANSI_4);
        KeyRegister::registerKey(utils::StringUtils::toUpper("5"), kVK_ANSI_5);
        KeyRegister::registerKey(utils::StringUtils::toUpper("6"), kVK_ANSI_6);
        KeyRegister::registerKey(utils::StringUtils::toUpper("7"), kVK_ANSI_7);
        KeyRegister::registerKey(utils::StringUtils::toUpper("8"), kVK_ANSI_8);
        KeyRegister::registerKey(utils::StringUtils::toUpper("9"), kVK_ANSI_9);

        // Function keys
        KeyRegister::registerKey(utils::StringUtils::toUpper("F1"), kVK_F1);
        KeyRegister::registerKey(utils::StringUtils::toUpper("F2"), kVK_F2);
        KeyRegister::registerKey(utils::StringUtils::toUpper("F3"), kVK_F3);
        KeyRegister::registerKey(utils::StringUtils::toUpper("F4"), kVK_F4);
        KeyRegister::registerKey(utils::StringUtils::toUpper("F5"), kVK_F5);
        KeyRegister::registerKey(utils::StringUtils::toUpper("F6"), kVK_F6);
        KeyRegister::registerKey(utils::StringUtils::toUpper("F7"), kVK_F7);
        KeyRegister::registerKey(utils::StringUtils::toUpper("F8"), kVK_F8);
        KeyRegister::registerKey(utils::StringUtils::toUpper("F9"), kVK_F9);
        KeyRegister::registerKey(utils::StringUtils::toUpper("F10"), kVK_F10);
        KeyRegister::registerKey(utils::StringUtils::toUpper("F11"), kVK_F11);
        KeyRegister::registerKey(utils::StringUtils::toUpper("F12"), kVK_F12);
        KeyRegister::registerKey(utils::StringUtils::toUpper("F13"), kVK_F13);
        KeyRegister::registerKey(utils::StringUtils::toUpper("F14"), kVK_F14);
        KeyRegister::registerKey(utils::StringUtils::toUpper("F15"), kVK_F15);
        KeyRegister::registerKey(utils::StringUtils::toUpper("F16"), kVK_F16);
        KeyRegister::registerKey(utils::StringUtils::toUpper("F17"), kVK_F17);
        KeyRegister::registerKey(utils::StringUtils::toUpper("F18"), kVK_F18);
        KeyRegister::registerKey(utils::StringUtils::toUpper("F19"), kVK_F19);
        KeyRegister::registerKey(utils::StringUtils::toUpper("F20"), kVK_F20);

        // Special keys
        KeyRegister::registerKey(utils::StringUtils::toUpper("Esc"), kVK_Escape);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Tab"), kVK_Tab);
        KeyRegister::registerKey(utils::StringUtils::toUpper("CapsLock"), kVK_CapsLock);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Space"), kVK_Space);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Backspace"), kVK_Delete);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Enter"), kVK_Return);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Del"), kVK_ForwardDelete);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Home"), kVK_Home);
        KeyRegister::registerKey(utils::StringUtils::toUpper("End"), kVK_End);
        KeyRegister::registerKey(utils::StringUtils::toUpper("PgUp"), kVK_PageUp);
        KeyRegister::registerKey(utils::StringUtils::toUpper("PgDn"), kVK_PageDown);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Help"), kVK_Help);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Clear"), kVK_ANSI_KeypadClear);
        KeyRegister::registerKey(utils::StringUtils::toUpper("PrtSc"), kVK_ANSI_KeypadClear);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Ins"), kVK_ANSI_KeypadClear);
        KeyRegister::registerKey(utils::StringUtils::toUpper("ScrLk"), kVK_ANSI_KeypadClear);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Pause"), kVK_ANSI_KeypadClear);

        // Arrow keys
        KeyRegister::registerKey(utils::StringUtils::toUpper("↑"), kVK_UpArrow);
        KeyRegister::registerKey(utils::StringUtils::toUpper("↓"), kVK_DownArrow);
        KeyRegister::registerKey(utils::StringUtils::toUpper("←"), kVK_LeftArrow);
        KeyRegister::registerKey(utils::StringUtils::toUpper("→"), kVK_RightArrow);

        // Modifier keys
        KeyRegister::registerKey(utils::StringUtils::toUpper("Ctrl"), kVK_Control);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Alt"), kVK_Option);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Shift"), kVK_Shift);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Left Ctrl"), kVK_Control);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Right Ctrl"), kVK_RightControl);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Left Shift"), kVK_Shift);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Right Shift"), kVK_RightShift);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Left Alt"), kVK_Option);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Right Alt"), kVK_RightOption);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Left Cmd"), kVK_Command);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Right Cmd"), kVK_RightCommand);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Cmd"), kVK_Command);

        // Additional keys
        KeyRegister::registerKey(utils::StringUtils::toUpper("/"), kVK_ANSI_Slash);
        KeyRegister::registerKey(utils::StringUtils::toUpper(";"), kVK_ANSI_Semicolon);
        KeyRegister::registerKey(utils::StringUtils::toUpper("="), kVK_ANSI_Equal);
        KeyRegister::registerKey(utils::StringUtils::toUpper("-"), kVK_ANSI_Minus);
        KeyRegister::registerKey(utils::StringUtils::toUpper("["), kVK_ANSI_LeftBracket);
        KeyRegister::registerKey(utils::StringUtils::toUpper("]"), kVK_ANSI_RightBracket);
        KeyRegister::registerKey(utils::StringUtils::toUpper("'"), kVK_ANSI_Quote);
        KeyRegister::registerKey(utils::StringUtils::toUpper(","), kVK_ANSI_Comma);
        KeyRegister::registerKey(utils::StringUtils::toUpper("."), kVK_ANSI_Period);
        KeyRegister::registerKey(utils::StringUtils::toUpper("\\"), kVK_ANSI_Backslash);
        KeyRegister::registerKey(utils::StringUtils::toUpper("`"), kVK_ANSI_Grave);

        // Keypad keys
        KeyRegister::registerKey(utils::StringUtils::toUpper("Num 0"), kVK_ANSI_Keypad0);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Num 1"), kVK_ANSI_Keypad1);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Num 2"), kVK_ANSI_Keypad2);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Num 3"), kVK_ANSI_Keypad3);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Num 4"), kVK_ANSI_Keypad4);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Num 5"), kVK_ANSI_Keypad5);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Num 6"), kVK_ANSI_Keypad6);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Num 7"), kVK_ANSI_Keypad7);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Num 8"), kVK_ANSI_Keypad8);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Num 9"), kVK_ANSI_Keypad9);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Num ."), kVK_ANSI_KeypadDecimal);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Num *"), kVK_ANSI_KeypadMultiply);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Num +"), kVK_ANSI_KeypadPlus);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Num -"), kVK_ANSI_KeypadMinus);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Num /"), kVK_ANSI_KeypadDivide);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Num Enter"), kVK_ANSI_KeypadEnter);
        KeyRegister::registerKey(utils::StringUtils::toUpper("NumLock"), kVK_ANSI_KeypadEquals);

        // Volume control keys
        KeyRegister::registerKey(utils::StringUtils::toUpper("Vol +"), kVK_VolumeUp);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Vol -"), kVK_VolumeDown);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Mute"), kVK_Mute);

        // Special masks
        KeyRegister::registerKey("KEY_PRESSED_MASK", 0x0100U);  // NOLINT
    }
} keyCodeInitializer;

}  // namespace interview_cheater::input::KeyCodes

#endif  // INTERVIEW_CHEATER_PLATFORM_MACOS_KEY_CODES_HPP