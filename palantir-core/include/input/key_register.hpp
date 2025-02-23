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
    std::unique_ptr<KeyRegisterImpl> pimpl;
#pragma warning(pop)

    // Private constructor for singleton
    KeyRegister();
    ~KeyRegister();
    
    // Delete copy and move operations
    KeyRegister(const KeyRegister&) = delete;
    KeyRegister& operator=(const KeyRegister&) = delete;
    KeyRegister(KeyRegister&&) = delete;
    KeyRegister& operator=(KeyRegister&&) = delete;

public:
    static KeyRegister& getInstance();
    
    void registerKey(const std::string& key, int value);
    [[nodiscard]] auto get(const std::string& key) const -> int;
    [[nodiscard]] auto hasKey(const std::string& key) const -> bool;
};

} // namespace interview_cheater::input

#endif // KEY_REGISTER_HPP