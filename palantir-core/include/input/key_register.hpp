#ifndef KEY_REGISTER_HPP
#define KEY_REGISTER_HPP

#include <memory>
#include <string>
#include "core_export.hpp"

namespace interview_cheater::input {

class KeyRegisterImpl;

/**
 * @class KeyRegister
 * @brief Singleton class for managing key registrations.
 *
 * This class provides a singleton interface for registering and
 * retrieving key values. It allows for key-value pair registration
 * and retrieval of registered values.
 */
class PALANTIR_CORE_API KeyRegister {
private:
    // Suppress C4251 warning for this specific line as Impl clas is never accessed by client
#pragma warning(push)
#pragma warning(disable: 4251)
    std::unique_ptr<KeyRegisterImpl> pimpl_;
#pragma warning(pop)

    // Private constructor for singleton
    KeyRegister();
    ~KeyRegister();

public:
    
    // Delete copy and move operations
    KeyRegister(const KeyRegister&) = delete;
    auto operator=(const KeyRegister&) -> KeyRegister& = delete;
    KeyRegister(KeyRegister&&) = delete;
    auto operator=(KeyRegister&&) -> KeyRegister& = delete;
    static auto getInstance() -> KeyRegister&;
    
    auto registerKey(const std::string& key, int value) -> void;
    [[nodiscard]] auto get(const std::string& key) const -> int;
    [[nodiscard]] auto hasKey(const std::string& key) const -> bool;
};

} // namespace interview_cheater::input

#endif // KEY_REGISTER_HPP