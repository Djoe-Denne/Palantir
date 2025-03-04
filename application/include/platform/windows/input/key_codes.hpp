#ifndef PALANTIR_KEY_CODES_HPP
#define PALANTIR_KEY_CODES_HPP

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
namespace palantir::input::KeyCodes {

// Initialize all key codes using KeyRegister
static const struct KeyCodeInitializer {
    KeyCodeInitializer() {  // NOLINT
        auto& keyRegister = KeyRegister::getInstance();

        // Letters
        keyRegister.registerKey(utils::StringUtils::toUpper("A"), 'A');
        keyRegister.registerKey(utils::StringUtils::toUpper("B"), 'B');
        keyRegister.registerKey(utils::StringUtils::toUpper("C"), 'C');
        keyRegister.registerKey(utils::StringUtils::toUpper("D"), 'D');
        keyRegister.registerKey(utils::StringUtils::toUpper("E"), 'E');
        keyRegister.registerKey(utils::StringUtils::toUpper("F"), 'F');
        keyRegister.registerKey(utils::StringUtils::toUpper("G"), 'G');
        keyRegister.registerKey(utils::StringUtils::toUpper("H"), 'H');
        keyRegister.registerKey(utils::StringUtils::toUpper("I"), 'I');
        keyRegister.registerKey(utils::StringUtils::toUpper("J"), 'J');
        keyRegister.registerKey(utils::StringUtils::toUpper("K"), 'K');
        keyRegister.registerKey(utils::StringUtils::toUpper("L"), 'L');
        keyRegister.registerKey(utils::StringUtils::toUpper("M"), 'M');
        keyRegister.registerKey(utils::StringUtils::toUpper("N"), 'N');
        keyRegister.registerKey(utils::StringUtils::toUpper("O"), 'O');
        keyRegister.registerKey(utils::StringUtils::toUpper("P"), 'P');
        keyRegister.registerKey(utils::StringUtils::toUpper("Q"), 'Q');
        keyRegister.registerKey(utils::StringUtils::toUpper("R"), 'R');
        keyRegister.registerKey(utils::StringUtils::toUpper("S"), 'S');
        keyRegister.registerKey(utils::StringUtils::toUpper("T"), 'T');
        keyRegister.registerKey(utils::StringUtils::toUpper("U"), 'U');
        keyRegister.registerKey(utils::StringUtils::toUpper("V"), 'V');
        keyRegister.registerKey(utils::StringUtils::toUpper("W"), 'W');
        keyRegister.registerKey(utils::StringUtils::toUpper("X"), 'X');
        keyRegister.registerKey(utils::StringUtils::toUpper("Y"), 'Y');
        keyRegister.registerKey(utils::StringUtils::toUpper("Z"), 'Z');

        // Numbers
        keyRegister.registerKey(utils::StringUtils::toUpper("0"), '0');
        keyRegister.registerKey(utils::StringUtils::toUpper("1"), '1');
        keyRegister.registerKey(utils::StringUtils::toUpper("2"), '2');
        keyRegister.registerKey(utils::StringUtils::toUpper("3"), '3');
        keyRegister.registerKey(utils::StringUtils::toUpper("4"), '4');
        keyRegister.registerKey(utils::StringUtils::toUpper("5"), '5');
        keyRegister.registerKey(utils::StringUtils::toUpper("6"), '6');
        keyRegister.registerKey(utils::StringUtils::toUpper("7"), '7');
        keyRegister.registerKey(utils::StringUtils::toUpper("8"), '8');
        keyRegister.registerKey(utils::StringUtils::toUpper("9"), '9');

        // Function keys
        keyRegister.registerKey(utils::StringUtils::toUpper("F1"), VK_F1);
        keyRegister.registerKey(utils::StringUtils::toUpper("F2"), VK_F2);
        keyRegister.registerKey(utils::StringUtils::toUpper("F3"), VK_F3);
        keyRegister.registerKey(utils::StringUtils::toUpper("F4"), VK_F4);
        keyRegister.registerKey(utils::StringUtils::toUpper("F5"), VK_F5);
        keyRegister.registerKey(utils::StringUtils::toUpper("F6"), VK_F6);
        keyRegister.registerKey(utils::StringUtils::toUpper("F7"), VK_F7);
        keyRegister.registerKey(utils::StringUtils::toUpper("F8"), VK_F8);
        keyRegister.registerKey(utils::StringUtils::toUpper("F9"), VK_F9);
        keyRegister.registerKey(utils::StringUtils::toUpper("F10"), VK_F10);
        keyRegister.registerKey(utils::StringUtils::toUpper("F11"), VK_F11);
        keyRegister.registerKey(utils::StringUtils::toUpper("F12"), VK_F12);
        keyRegister.registerKey(utils::StringUtils::toUpper("F13"), VK_F13);
        keyRegister.registerKey(utils::StringUtils::toUpper("F14"), VK_F14);
        keyRegister.registerKey(utils::StringUtils::toUpper("F15"), VK_F15);
        keyRegister.registerKey(utils::StringUtils::toUpper("F16"), VK_F16);
        keyRegister.registerKey(utils::StringUtils::toUpper("F17"), VK_F17);
        keyRegister.registerKey(utils::StringUtils::toUpper("F18"), VK_F18);
        keyRegister.registerKey(utils::StringUtils::toUpper("F19"), VK_F19);
        keyRegister.registerKey(utils::StringUtils::toUpper("F20"), VK_F20);

        // Special keys
        keyRegister.registerKey(utils::StringUtils::toUpper("Esc"), VK_ESCAPE);
        keyRegister.registerKey(utils::StringUtils::toUpper("Tab"), VK_TAB);
        keyRegister.registerKey(utils::StringUtils::toUpper("CapsLock"), VK_CAPITAL);
        keyRegister.registerKey(utils::StringUtils::toUpper("Space"), VK_SPACE);
        keyRegister.registerKey(utils::StringUtils::toUpper("Backspace"), VK_BACK);
        keyRegister.registerKey(utils::StringUtils::toUpper("Enter"), VK_RETURN);
        keyRegister.registerKey(utils::StringUtils::toUpper("Del"), VK_DELETE);
        keyRegister.registerKey(utils::StringUtils::toUpper("Home"), VK_HOME);
        keyRegister.registerKey(utils::StringUtils::toUpper("End"), VK_END);
        keyRegister.registerKey(utils::StringUtils::toUpper("PgUp"), VK_PRIOR);
        keyRegister.registerKey(utils::StringUtils::toUpper("PgDn"), VK_NEXT);
        keyRegister.registerKey(utils::StringUtils::toUpper("Help"), VK_HELP);
        keyRegister.registerKey(utils::StringUtils::toUpper("Clear"), VK_CLEAR);
        keyRegister.registerKey(utils::StringUtils::toUpper("PrtSc"), VK_SNAPSHOT);
        keyRegister.registerKey(utils::StringUtils::toUpper("Ins"), VK_INSERT);
        keyRegister.registerKey(utils::StringUtils::toUpper("ScrLk"), VK_SCROLL);
        keyRegister.registerKey(utils::StringUtils::toUpper("Pause"), VK_PAUSE);

        // Arrow keys
        keyRegister.registerKey(utils::StringUtils::toUpper("↑"), VK_UP);
        keyRegister.registerKey(utils::StringUtils::toUpper("↓"), VK_DOWN);
        keyRegister.registerKey(utils::StringUtils::toUpper("←"), VK_LEFT);
        keyRegister.registerKey(utils::StringUtils::toUpper("→"), VK_RIGHT);

        // Modifier keys
        keyRegister.registerKey(utils::StringUtils::toUpper("Ctrl"), VK_CONTROL);
        keyRegister.registerKey(utils::StringUtils::toUpper("Alt"), VK_MENU);
        keyRegister.registerKey(utils::StringUtils::toUpper("Shift"), VK_SHIFT);
        keyRegister.registerKey(utils::StringUtils::toUpper("Left Ctrl"), VK_LCONTROL);
        keyRegister.registerKey(utils::StringUtils::toUpper("Right Ctrl"), VK_RCONTROL);
        keyRegister.registerKey(utils::StringUtils::toUpper("Left Shift"), VK_LSHIFT);
        keyRegister.registerKey(utils::StringUtils::toUpper("Right Shift"), VK_RSHIFT);
        keyRegister.registerKey(utils::StringUtils::toUpper("Left Alt"), VK_LMENU);
        keyRegister.registerKey(utils::StringUtils::toUpper("Right Alt"), VK_RMENU);
        keyRegister.registerKey(utils::StringUtils::toUpper("Left Win"), VK_LWIN);
        keyRegister.registerKey(utils::StringUtils::toUpper("Right Win"), VK_RWIN);
        keyRegister.registerKey(utils::StringUtils::toUpper("Win"), VK_LWIN);

        // Additional keys
        keyRegister.registerKey(utils::StringUtils::toUpper("/"), VK_OEM_2);
        keyRegister.registerKey(utils::StringUtils::toUpper(";"), VK_OEM_1);
        keyRegister.registerKey(utils::StringUtils::toUpper("="), VK_OEM_PLUS);
        keyRegister.registerKey(utils::StringUtils::toUpper("-"), VK_OEM_MINUS);
        keyRegister.registerKey(utils::StringUtils::toUpper("["), VK_OEM_4);
        keyRegister.registerKey(utils::StringUtils::toUpper("]"), VK_OEM_6);
        keyRegister.registerKey(utils::StringUtils::toUpper("'"), VK_OEM_7);
        keyRegister.registerKey(utils::StringUtils::toUpper(","), VK_OEM_COMMA);
        keyRegister.registerKey(utils::StringUtils::toUpper("."), VK_OEM_PERIOD);
        keyRegister.registerKey(utils::StringUtils::toUpper("\\"), VK_OEM_5);
        keyRegister.registerKey(utils::StringUtils::toUpper("`"), VK_OEM_3);

        // Keypad keys
        keyRegister.registerKey(utils::StringUtils::toUpper("Num 0"), VK_NUMPAD0);
        keyRegister.registerKey(utils::StringUtils::toUpper("Num 1"), VK_NUMPAD1);
        keyRegister.registerKey(utils::StringUtils::toUpper("Num 2"), VK_NUMPAD2);
        keyRegister.registerKey(utils::StringUtils::toUpper("Num 3"), VK_NUMPAD3);
        keyRegister.registerKey(utils::StringUtils::toUpper("Num 4"), VK_NUMPAD4);
        keyRegister.registerKey(utils::StringUtils::toUpper("Num 5"), VK_NUMPAD5);
        keyRegister.registerKey(utils::StringUtils::toUpper("Num 6"), VK_NUMPAD6);
        keyRegister.registerKey(utils::StringUtils::toUpper("Num 7"), VK_NUMPAD7);
        keyRegister.registerKey(utils::StringUtils::toUpper("Num 8"), VK_NUMPAD8);
        keyRegister.registerKey(utils::StringUtils::toUpper("Num 9"), VK_NUMPAD9);
        keyRegister.registerKey(utils::StringUtils::toUpper("Num ."), VK_DECIMAL);
        keyRegister.registerKey(utils::StringUtils::toUpper("Num *"), VK_MULTIPLY);
        keyRegister.registerKey(utils::StringUtils::toUpper("Num +"), VK_ADD);
        keyRegister.registerKey(utils::StringUtils::toUpper("Num -"), VK_SUBTRACT);
        keyRegister.registerKey(utils::StringUtils::toUpper("Num /"), VK_DIVIDE);
        keyRegister.registerKey(utils::StringUtils::toUpper("Num Enter"), VK_RETURN);
        keyRegister.registerKey(utils::StringUtils::toUpper("NumLock"), VK_NUMLOCK);

        // Special masks
        keyRegister.registerKey("KEY_PRESSED_MASK", 0x8000U);  // NOLINT
    }
} keyCodeInitializer;  // NOLINT

}  // namespace palantir::input::KeyCodes

#endif  // PALANTIR_KEY_CODES_HPP