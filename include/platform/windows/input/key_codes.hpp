#ifndef INTERVIEW_CHEATER_KEY_CODES_HPP
#define INTERVIEW_CHEATER_KEY_CODES_HPP

#include <windows.h>

#include "input/key_register.hpp"
#include "utils/string_utils.hpp"
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

// Initialize all key codes using KeyRegister
static const struct KeyCodeInitializer {
    KeyCodeInitializer() {  // NOLINT
        // Letters
        KeyRegister::registerKey(utils::StringUtils::toUpper("A"), 'A');
        KeyRegister::registerKey(utils::StringUtils::toUpper("B"), 'B');
        KeyRegister::registerKey(utils::StringUtils::toUpper("C"), 'C');
        KeyRegister::registerKey(utils::StringUtils::toUpper("D"), 'D');
        KeyRegister::registerKey(utils::StringUtils::toUpper("E"), 'E');
        KeyRegister::registerKey(utils::StringUtils::toUpper("F"), 'F');
        KeyRegister::registerKey(utils::StringUtils::toUpper("G"), 'G');
        KeyRegister::registerKey(utils::StringUtils::toUpper("H"), 'H');
        KeyRegister::registerKey(utils::StringUtils::toUpper("I"), 'I');
        KeyRegister::registerKey(utils::StringUtils::toUpper("J"), 'J');
        KeyRegister::registerKey(utils::StringUtils::toUpper("K"), 'K');
        KeyRegister::registerKey(utils::StringUtils::toUpper("L"), 'L');
        KeyRegister::registerKey(utils::StringUtils::toUpper("M"), 'M');
        KeyRegister::registerKey(utils::StringUtils::toUpper("N"), 'N');
        KeyRegister::registerKey(utils::StringUtils::toUpper("O"), 'O');
        KeyRegister::registerKey(utils::StringUtils::toUpper("P"), 'P');
        KeyRegister::registerKey(utils::StringUtils::toUpper("Q"), 'Q');
        KeyRegister::registerKey(utils::StringUtils::toUpper("R"), 'R');
        KeyRegister::registerKey(utils::StringUtils::toUpper("S"), 'S');
        KeyRegister::registerKey(utils::StringUtils::toUpper("T"), 'T');
        KeyRegister::registerKey(utils::StringUtils::toUpper("U"), 'U');
        KeyRegister::registerKey(utils::StringUtils::toUpper("V"), 'V');
        KeyRegister::registerKey(utils::StringUtils::toUpper("W"), 'W');
        KeyRegister::registerKey(utils::StringUtils::toUpper("X"), 'X');
        KeyRegister::registerKey(utils::StringUtils::toUpper("Y"), 'Y');
        KeyRegister::registerKey(utils::StringUtils::toUpper("Z"), 'Z');

        // Numbers
        KeyRegister::registerKey(utils::StringUtils::toUpper("0"), '0');
        KeyRegister::registerKey(utils::StringUtils::toUpper("1"), '1');
        KeyRegister::registerKey(utils::StringUtils::toUpper("2"), '2');
        KeyRegister::registerKey(utils::StringUtils::toUpper("3"), '3');
        KeyRegister::registerKey(utils::StringUtils::toUpper("4"), '4');
        KeyRegister::registerKey(utils::StringUtils::toUpper("5"), '5');
        KeyRegister::registerKey(utils::StringUtils::toUpper("6"), '6');
        KeyRegister::registerKey(utils::StringUtils::toUpper("7"), '7');
        KeyRegister::registerKey(utils::StringUtils::toUpper("8"), '8');
        KeyRegister::registerKey(utils::StringUtils::toUpper("9"), '9');

        // Function keys
        KeyRegister::registerKey(utils::StringUtils::toUpper("F1"), VK_F1);
        KeyRegister::registerKey(utils::StringUtils::toUpper("F2"), VK_F2);
        KeyRegister::registerKey(utils::StringUtils::toUpper("F3"), VK_F3);
        KeyRegister::registerKey(utils::StringUtils::toUpper("F4"), VK_F4);
        KeyRegister::registerKey(utils::StringUtils::toUpper("F5"), VK_F5);
        KeyRegister::registerKey(utils::StringUtils::toUpper("F6"), VK_F6);
        KeyRegister::registerKey(utils::StringUtils::toUpper("F7"), VK_F7);
        KeyRegister::registerKey(utils::StringUtils::toUpper("F8"), VK_F8);
        KeyRegister::registerKey(utils::StringUtils::toUpper("F9"), VK_F9);
        KeyRegister::registerKey(utils::StringUtils::toUpper("F10"), VK_F10);
        KeyRegister::registerKey(utils::StringUtils::toUpper("F11"), VK_F11);
        KeyRegister::registerKey(utils::StringUtils::toUpper("F12"), VK_F12);
        KeyRegister::registerKey(utils::StringUtils::toUpper("F13"), VK_F13);
        KeyRegister::registerKey(utils::StringUtils::toUpper("F14"), VK_F14);
        KeyRegister::registerKey(utils::StringUtils::toUpper("F15"), VK_F15);
        KeyRegister::registerKey(utils::StringUtils::toUpper("F16"), VK_F16);
        KeyRegister::registerKey(utils::StringUtils::toUpper("F17"), VK_F17);
        KeyRegister::registerKey(utils::StringUtils::toUpper("F18"), VK_F18);
        KeyRegister::registerKey(utils::StringUtils::toUpper("F19"), VK_F19);
        KeyRegister::registerKey(utils::StringUtils::toUpper("F20"), VK_F20);

        // Special keys
        KeyRegister::registerKey(utils::StringUtils::toUpper("Esc"), VK_ESCAPE);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Tab"), VK_TAB);
        KeyRegister::registerKey(utils::StringUtils::toUpper("CapsLock"), VK_CAPITAL);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Space"), VK_SPACE);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Backspace"), VK_BACK);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Enter"), VK_RETURN);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Del"), VK_DELETE);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Home"), VK_HOME);
        KeyRegister::registerKey(utils::StringUtils::toUpper("End"), VK_END);
        KeyRegister::registerKey(utils::StringUtils::toUpper("PgUp"), VK_PRIOR);
        KeyRegister::registerKey(utils::StringUtils::toUpper("PgDn"), VK_NEXT);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Help"), VK_HELP);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Clear"), VK_CLEAR);
        KeyRegister::registerKey(utils::StringUtils::toUpper("PrtSc"), VK_SNAPSHOT);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Ins"), VK_INSERT);
        KeyRegister::registerKey(utils::StringUtils::toUpper("ScrLk"), VK_SCROLL);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Pause"), VK_PAUSE);

        // Arrow keys
        KeyRegister::registerKey(utils::StringUtils::toUpper("↑"), VK_UP);
        KeyRegister::registerKey(utils::StringUtils::toUpper("↓"), VK_DOWN);
        KeyRegister::registerKey(utils::StringUtils::toUpper("←"), VK_LEFT);
        KeyRegister::registerKey(utils::StringUtils::toUpper("→"), VK_RIGHT);

        // Modifier keys
        KeyRegister::registerKey(utils::StringUtils::toUpper("Ctrl"), VK_CONTROL);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Alt"), VK_MENU);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Shift"), VK_SHIFT);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Left Ctrl"), VK_LCONTROL);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Right Ctrl"), VK_RCONTROL);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Left Shift"), VK_LSHIFT);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Right Shift"), VK_RSHIFT);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Left Alt"), VK_LMENU);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Right Alt"), VK_RMENU);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Left Win"), VK_LWIN);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Right Win"), VK_RWIN);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Win"), VK_LWIN);

        // Additional keys
        KeyRegister::registerKey(utils::StringUtils::toUpper("/"), VK_OEM_2);
        KeyRegister::registerKey(utils::StringUtils::toUpper(";"), VK_OEM_1);
        KeyRegister::registerKey(utils::StringUtils::toUpper("="), VK_OEM_PLUS);
        KeyRegister::registerKey(utils::StringUtils::toUpper("-"), VK_OEM_MINUS);
        KeyRegister::registerKey(utils::StringUtils::toUpper("["), VK_OEM_4);
        KeyRegister::registerKey(utils::StringUtils::toUpper("]"), VK_OEM_6);
        KeyRegister::registerKey(utils::StringUtils::toUpper("'"), VK_OEM_7);
        KeyRegister::registerKey(utils::StringUtils::toUpper(","), VK_OEM_COMMA);
        KeyRegister::registerKey(utils::StringUtils::toUpper("."), VK_OEM_PERIOD);
        KeyRegister::registerKey(utils::StringUtils::toUpper("\\"), VK_OEM_5);
        KeyRegister::registerKey(utils::StringUtils::toUpper("`"), VK_OEM_3);

        // Keypad keys
        KeyRegister::registerKey(utils::StringUtils::toUpper("Num 0"), VK_NUMPAD0);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Num 1"), VK_NUMPAD1);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Num 2"), VK_NUMPAD2);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Num 3"), VK_NUMPAD3);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Num 4"), VK_NUMPAD4);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Num 5"), VK_NUMPAD5);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Num 6"), VK_NUMPAD6);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Num 7"), VK_NUMPAD7);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Num 8"), VK_NUMPAD8);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Num 9"), VK_NUMPAD9);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Num ."), VK_DECIMAL);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Num *"), VK_MULTIPLY);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Num +"), VK_ADD);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Num -"), VK_SUBTRACT);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Num /"), VK_DIVIDE);
        KeyRegister::registerKey(utils::StringUtils::toUpper("Num Enter"), VK_RETURN);
        KeyRegister::registerKey(utils::StringUtils::toUpper("NumLock"), VK_NUMLOCK);

        // Special masks
        KeyRegister::registerKey("KEY_PRESSED_MASK", 0x8000U);  // NOLINT
    }
} keyCodeInitializer;  // NOLINT

}  // namespace interview_cheater::input::KeyCodes

#endif  // INTERVIEW_CHEATER_KEY_CODES_HPP