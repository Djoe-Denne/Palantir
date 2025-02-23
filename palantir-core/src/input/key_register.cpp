#include "input/key_register.hpp"  // Include the corresponding header file

#include <map>
#include <stdexcept>
#include <string>

#include "utils/string_utils.hpp"

namespace interview_cheater::input {

class KeyRegisterImpl {
public:
    std::map<std::string, int> keyMap;

    void registerKey(const std::string& key, int value) { keyMap[utils::StringUtils::toUpper(key)] = value; }

    auto get(const std::string& key) const -> int {
        try {
            return keyMap.at(utils::StringUtils::toUpper(key));
        } catch (const std::out_of_range&) {
            throw std::out_of_range("Key not found: " + key);
        }
    }

    auto hasKey(const std::string& key) const -> bool {
        return keyMap.find(utils::StringUtils::toUpper(key)) != keyMap.end();
    }
};

// Constructor and destructor
KeyRegister::KeyRegister() : pimpl(std::make_unique<KeyRegisterImpl>()) {}
KeyRegister::~KeyRegister() = default;

// Singleton instance getter
KeyRegister& KeyRegister::getInstance() {
    static KeyRegister instance;
    return instance;
}

// Public interface implementation
void KeyRegister::registerKey(const std::string& key, int value) { pimpl->registerKey(key, value); }

auto KeyRegister::get(const std::string& key) const -> int { return pimpl->get(key); }

auto KeyRegister::hasKey(const std::string& key) const -> bool { return pimpl->hasKey(key); }
}  // namespace interview_cheater::input