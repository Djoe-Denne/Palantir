#include "input/key_register.hpp"  // Include the corresponding header file

#include <map>
#include <stdexcept>
#include <string>

#include "utils/string_utils.hpp"

namespace interview_cheater::input {

// Initialize the static member outside the class
std::map<std::string, int> KeyRegister::keyMap;

// Register a key-value pair (static method)
void KeyRegister::registerKey(const std::string& key, int value) { keyMap[key] = value; }

// Get the value associated with a key (static method)
auto KeyRegister::get(const std::string& key) -> int {
    try {
        return keyMap.at(utils::StringUtils::toUpper(key));  // Use .at() for bounds-checked access
    } catch (const std::out_of_range&) {
        throw std::out_of_range("Key not found: " + key);
    }
}

auto KeyRegister::hasKey(const std::string& key) -> bool {
    return keyMap.find(utils::StringUtils::toUpper(key)) != keyMap.end();
}
}  // namespace interview_cheater::input