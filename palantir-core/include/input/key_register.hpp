#ifndef KEY_REGISTER_HPP
#define KEY_REGISTER_HPP

#include <memory>
#include <string>
#include "core_export.hpp"

namespace palantir::input {

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
protected:
    // Suppress C4251 warning for this specific line as Impl clas is never accessed by client
#pragma warning(push)
#pragma warning(disable: 4251)
    std::unique_ptr<KeyRegisterImpl> pimpl_;

    static std::shared_ptr<KeyRegister> instance_;
#pragma warning(pop)

    // Private constructor for singleton
    KeyRegister();

public:
    virtual ~KeyRegister();
    
    // Delete copy and move operations
    KeyRegister(const KeyRegister&) = delete;
    auto operator=(const KeyRegister&) -> KeyRegister& = delete;
    KeyRegister(KeyRegister&&) = delete;
    auto operator=(KeyRegister&&) -> KeyRegister& = delete;
    static auto getInstance() -> std::shared_ptr<KeyRegister>;
    static auto setInstance(const std::shared_ptr<KeyRegister>& instance) -> void;
    
    virtual auto registerKey(const std::string& key, int value) -> void;
    [[nodiscard]] virtual auto get(const std::string& key) const -> int;
    [[nodiscard]] virtual auto hasKey(const std::string& key) const -> bool;
};

} // namespace palantir::input

#endif // KEY_REGISTER_HPP