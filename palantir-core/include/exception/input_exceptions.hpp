#pragma once

#include "base_exception.hpp"

namespace palantir::exception {

/**
 * @brief Thrown when the input factory is not initialized
 */
class PALANTIR_CORE_API InputFactoryException : public BaseException {
public:
    explicit InputFactoryException(
        const std::string& message = "InputFactory not initialized. Call initialize() first.")
        : BaseException(message) {}
};

/**
 * @brief Thrown when there are errors in shortcut parsing and handling
 */
class PALANTIR_CORE_API ShortcutConfigurationException : public BaseException {
public:
    explicit ShortcutConfigurationException(const std::string& message = "Invalid shortcut configuration")
        : BaseException(message) {}
};

}  // namespace palantir::exception