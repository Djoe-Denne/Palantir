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
        auto& keyRegister = KeyRegister::getInstance();
        
        // Letters
        keyRegister.registerKey(utils::StringUtils::toUpper("A"), kVK_ANSI_A);
        keyRegister.registerKey(utils::StringUtils::toUpper("B"), kVK_ANSI_B);
        keyRegister.registerKey(utils::StringUtils::toUpper("C"), kVK_ANSI_C);
        keyRegister.registerKey(utils::StringUtils::toUpper("D"), kVK_ANSI_D);
        keyRegister.registerKey(utils::StringUtils::toUpper("E"), kVK_ANSI_E);
        keyRegister.registerKey(utils::StringUtils::toUpper("F"), kVK_ANSI_F);
        keyRegister.registerKey(utils::StringUtils::toUpper("G"), kVK_ANSI_G);
        keyRegister.registerKey(utils::StringUtils::toUpper("H"), kVK_ANSI_H);
        keyRegister.registerKey(utils::StringUtils::toUpper("I"), kVK_ANSI_I);
        keyRegister.registerKey(utils::StringUtils::toUpper("J"), kVK_ANSI_J);
        keyRegister.registerKey(utils::StringUtils::toUpper("K"), kVK_ANSI_K);
        keyRegister.registerKey(utils::StringUtils::toUpper("L"), kVK_ANSI_L);
        keyRegister.registerKey(utils::StringUtils::toUpper("M"), kVK_ANSI_M);
        keyRegister.registerKey(utils::StringUtils::toUpper("N"), kVK_ANSI_N);
        keyRegister.registerKey(utils::StringUtils::toUpper("O"), kVK_ANSI_O);
        keyRegister.registerKey(utils::StringUtils::toUpper("P"), kVK_ANSI_P);
        keyRegister.registerKey(utils::StringUtils::toUpper("Q"), kVK_ANSI_Q);
        keyRegister.registerKey(utils::StringUtils::toUpper("R"), kVK_ANSI_R);
        keyRegister.registerKey(utils::StringUtils::toUpper("S"), kVK_ANSI_S);
        keyRegister.registerKey(utils::StringUtils::toUpper("T"), kVK_ANSI_T);
        keyRegister.registerKey(utils::StringUtils::toUpper("U"), kVK_ANSI_U);
        keyRegister.registerKey(utils::StringUtils::toUpper("V"), kVK_ANSI_V);
        keyRegister.registerKey(utils::StringUtils::toUpper("W"), kVK_ANSI_W);
        keyRegister.registerKey(utils::StringUtils::toUpper("X"), kVK_ANSI_X);
        keyRegister.registerKey(utils::StringUtils::toUpper("Y"), kVK_ANSI_Y);
        keyRegister.registerKey(utils::StringUtils::toUpper("Z"), kVK_ANSI_Z);

        // Numbers
        keyRegister.registerKey(utils::StringUtils::toUpper("0"), kVK_ANSI_0);
        keyRegister.registerKey(utils::StringUtils::toUpper("1"), kVK_ANSI_1);
        keyRegister.registerKey(utils::StringUtils::toUpper("2"), kVK_ANSI_2);
        keyRegister.registerKey(utils::StringUtils::toUpper("3"), kVK_ANSI_3);
        keyRegister.registerKey(utils::StringUtils::toUpper("4"), kVK_ANSI_4);
        keyRegister.registerKey(utils::StringUtils::toUpper("5"), kVK_ANSI_5);
        keyRegister.registerKey(utils::StringUtils::toUpper("6"), kVK_ANSI_6);
        keyRegister.registerKey(utils::StringUtils::toUpper("7"), kVK_ANSI_7);
        keyRegister.registerKey(utils::StringUtils::toUpper("8"), kVK_ANSI_8);
        keyRegister.registerKey(utils::StringUtils::toUpper("9"), kVK_ANSI_9);

        // Function keys
        keyRegister.registerKey(utils::StringUtils::toUpper("F1"), kVK_F1);
        keyRegister.registerKey(utils::StringUtils::toUpper("F2"), kVK_F2);
        keyRegister.registerKey(utils::StringUtils::toUpper("F3"), kVK_F3);
        keyRegister.registerKey(utils::StringUtils::toUpper("F4"), kVK_F4);
        keyRegister.registerKey(utils::StringUtils::toUpper("F5"), kVK_F5);
        keyRegister.registerKey(utils::StringUtils::toUpper("F6"), kVK_F6);
        keyRegister.registerKey(utils::StringUtils::toUpper("F7"), kVK_F7);
        keyRegister.registerKey(utils::StringUtils::toUpper("F8"), kVK_F8);
        keyRegister.registerKey(utils::StringUtils::toUpper("F9"), kVK_F9);
        keyRegister.registerKey(utils::StringUtils::toUpper("F10"), kVK_F10);
        keyRegister.registerKey(utils::StringUtils::toUpper("F11"), kVK_F11);
        keyRegister.registerKey(utils::StringUtils::toUpper("F12"), kVK_F12);
        keyRegister.registerKey(utils::StringUtils::toUpper("F13"), kVK_F13);
        keyRegister.registerKey(utils::StringUtils::toUpper("F14"), kVK_F14);
        keyRegister.registerKey(utils::StringUtils::toUpper("F15"), kVK_F15);
        keyRegister.registerKey(utils::StringUtils::toUpper("F16"), kVK_F16);
        keyRegister.registerKey(utils::StringUtils::toUpper("F17"), kVK_F17);
        keyRegister.registerKey(utils::StringUtils::toUpper("F18"), kVK_F18);
        keyRegister.registerKey(utils::StringUtils::toUpper("F19"), kVK_F19);
        keyRegister.registerKey(utils::StringUtils::toUpper("F20"), kVK_F20);

        // Special keys
        keyRegister.registerKey(utils::StringUtils::toUpper("Esc"), kVK_Escape);
        keyRegister.registerKey(utils::StringUtils::toUpper("Tab"), kVK_Tab);
        keyRegister.registerKey(utils::StringUtils::toUpper("CapsLock"), kVK_CapsLock);
        keyRegister.registerKey(utils::StringUtils::toUpper("Space"), kVK_Space);
        keyRegister.registerKey(utils::StringUtils::toUpper("Backspace"), kVK_Delete);
        keyRegister.registerKey(utils::StringUtils::toUpper("Enter"), kVK_Return);
        keyRegister.registerKey(utils::StringUtils::toUpper("Del"), kVK_ForwardDelete);
        keyRegister.registerKey(utils::StringUtils::toUpper("Home"), kVK_Home);
        keyRegister.registerKey(utils::StringUtils::toUpper("End"), kVK_End);
        keyRegister.registerKey(utils::StringUtils::toUpper("PgUp"), kVK_PageUp);
        keyRegister.registerKey(utils::StringUtils::toUpper("PgDn"), kVK_PageDown);
        keyRegister.registerKey(utils::StringUtils::toUpper("Help"), kVK_Help);
        keyRegister.registerKey(utils::StringUtils::toUpper("Clear"), kVK_ANSI_KeypadClear);
        keyRegister.registerKey(utils::StringUtils::toUpper("PrtSc"), kVK_ANSI_KeypadClear);
        keyRegister.registerKey(utils::StringUtils::toUpper("Ins"), kVK_ANSI_KeypadClear);
        keyRegister.registerKey(utils::StringUtils::toUpper("ScrLk"), kVK_ANSI_KeypadClear);
        keyRegister.registerKey(utils::StringUtils::toUpper("Pause"), kVK_ANSI_KeypadClear);

        // Arrow keys
        keyRegister.registerKey(utils::StringUtils::toUpper("↑"), kVK_UpArrow);
        keyRegister.registerKey(utils::StringUtils::toUpper("↓"), kVK_DownArrow);
        keyRegister.registerKey(utils::StringUtils::toUpper("←"), kVK_LeftArrow);
        keyRegister.registerKey(utils::StringUtils::toUpper("→"), kVK_RightArrow);

        // Modifier keys
        keyRegister.registerKey(utils::StringUtils::toUpper("Ctrl"), kVK_Control);
        keyRegister.registerKey(utils::StringUtils::toUpper("Alt"), kVK_Option);
        keyRegister.registerKey(utils::StringUtils::toUpper("Shift"), kVK_Shift);
        keyRegister.registerKey(utils::StringUtils::toUpper("Left Ctrl"), kVK_Control);
        keyRegister.registerKey(utils::StringUtils::toUpper("Right Ctrl"), kVK_RightControl);
        keyRegister.registerKey(utils::StringUtils::toUpper("Left Shift"), kVK_Shift);
        keyRegister.registerKey(utils::StringUtils::toUpper("Right Shift"), kVK_RightShift);
        keyRegister.registerKey(utils::StringUtils::toUpper("Left Alt"), kVK_Option);
        keyRegister.registerKey(utils::StringUtils::toUpper("Right Alt"), kVK_RightOption);
        keyRegister.registerKey(utils::StringUtils::toUpper("Left Cmd"), kVK_Command);
        keyRegister.registerKey(utils::StringUtils::toUpper("Right Cmd"), kVK_RightCommand);
        keyRegister.registerKey(utils::StringUtils::toUpper("Cmd"), kVK_Command);

        // Additional keys
        keyRegister.registerKey(utils::StringUtils::toUpper("/"), kVK_ANSI_Slash);
        keyRegister.registerKey(utils::StringUtils::toUpper(";"), kVK_ANSI_Semicolon);
        keyRegister.registerKey(utils::StringUtils::toUpper("="), kVK_ANSI_Equal);
        keyRegister.registerKey(utils::StringUtils::toUpper("-"), kVK_ANSI_Minus);
        keyRegister.registerKey(utils::StringUtils::toUpper("["), kVK_ANSI_LeftBracket);
        keyRegister.registerKey(utils::StringUtils::toUpper("]"), kVK_ANSI_RightBracket);
        keyRegister.registerKey(utils::StringUtils::toUpper("'"), kVK_ANSI_Quote);
        keyRegister.registerKey(utils::StringUtils::toUpper(","), kVK_ANSI_Comma);
        keyRegister.registerKey(utils::StringUtils::toUpper("."), kVK_ANSI_Period);
        keyRegister.registerKey(utils::StringUtils::toUpper("\\"), kVK_ANSI_Backslash);
        keyRegister.registerKey(utils::StringUtils::toUpper("`"), kVK_ANSI_Grave);

        // Keypad keys
        keyRegister.registerKey(utils::StringUtils::toUpper("Num 0"), kVK_ANSI_Keypad0);
        keyRegister.registerKey(utils::StringUtils::toUpper("Num 1"), kVK_ANSI_Keypad1);
        keyRegister.registerKey(utils::StringUtils::toUpper("Num 2"), kVK_ANSI_Keypad2);
        keyRegister.registerKey(utils::StringUtils::toUpper("Num 3"), kVK_ANSI_Keypad3);
        keyRegister.registerKey(utils::StringUtils::toUpper("Num 4"), kVK_ANSI_Keypad4);
        keyRegister.registerKey(utils::StringUtils::toUpper("Num 5"), kVK_ANSI_Keypad5);
        keyRegister.registerKey(utils::StringUtils::toUpper("Num 6"), kVK_ANSI_Keypad6);
        keyRegister.registerKey(utils::StringUtils::toUpper("Num 7"), kVK_ANSI_Keypad7);
        keyRegister.registerKey(utils::StringUtils::toUpper("Num 8"), kVK_ANSI_Keypad8);
        keyRegister.registerKey(utils::StringUtils::toUpper("Num 9"), kVK_ANSI_Keypad9);
        keyRegister.registerKey(utils::StringUtils::toUpper("Num ."), kVK_ANSI_KeypadDecimal);
        keyRegister.registerKey(utils::StringUtils::toUpper("Num *"), kVK_ANSI_KeypadMultiply);
        keyRegister.registerKey(utils::StringUtils::toUpper("Num +"), kVK_ANSI_KeypadPlus);
        keyRegister.registerKey(utils::StringUtils::toUpper("Num -"), kVK_ANSI_KeypadMinus);
        keyRegister.registerKey(utils::StringUtils::toUpper("Num /"), kVK_ANSI_KeypadDivide);
        keyRegister.registerKey(utils::StringUtils::toUpper("Num Enter"), kVK_ANSI_KeypadEnter);
        keyRegister.registerKey(utils::StringUtils::toUpper("NumLock"), kVK_ANSI_KeypadEquals);

        // Volume control keys
        keyRegister.registerKey(utils::StringUtils::toUpper("Vol +"), kVK_VolumeUp);
        keyRegister.registerKey(utils::StringUtils::toUpper("Vol -"), kVK_VolumeDown);
        keyRegister.registerKey(utils::StringUtils::toUpper("Mute"), kVK_Mute);

        // Special masks
        keyRegister.registerKey("KEY_PRESSED_MASK", 0x0100U);  // NOLINT
    }
} keyCodeInitializer;

}  // namespace interview_cheater::input::KeyCodes

#endif  // INTERVIEW_CHEATER_PLATFORM_MACOS_KEY_CODES_HPP