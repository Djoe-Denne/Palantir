#include "input/key_register.hpp"  // Include the corresponding header file

#include <map>
#include <stdexcept>
#include <string>

#include "utils/string_utils.hpp"

namespace palantir::input {

class KeyRegisterImpl {
public:
    std::map<std::string, int> keyMap;

    auto registerKey(const std::string& key, int value) -> void { keyMap[utils::StringUtils::toUpper(key)] = value; }

    [[nodiscard]] auto get(const std::string& key) const -> int {
        try {
            return keyMap.at(utils::StringUtils::toUpper(key));
        } catch (const std::out_of_range&) {
            throw std::out_of_range("Key not found: " + key);
        }
    }

    [[nodiscard]] auto hasKey(const std::string& key) const -> bool {
        return keyMap.find(utils::StringUtils::toUpper(key)) != keyMap.end();
    }
};

// Constructor and destructor
KeyRegister::KeyRegister() : pimpl_(std::make_unique<KeyRegisterImpl>()) {}
KeyRegister::~KeyRegister() = default;

// Singleton instance getter
auto KeyRegister::getInstance() -> KeyRegister& {
    static KeyRegister instance;
    return instance;
}

// Public interface implementation
void KeyRegister::registerKey(const std::string& key, int value) { pimpl_->registerKey(key, value); }

auto KeyRegister::get(const std::string& key) const -> int { return pimpl_->get(key); }

auto KeyRegister::hasKey(const std::string& key) const -> bool { return pimpl_->hasKey(key); }
}  // namespace palantir::input