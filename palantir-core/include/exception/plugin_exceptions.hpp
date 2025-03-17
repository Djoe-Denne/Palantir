#pragma once

#include "base_exception.hpp"

namespace palantir::exception {

/**
 * @brief Thrown when plugin initialization fails
 */
class PALANTIR_CORE_API PluginInitializationException : public BaseException {
public:
    explicit PluginInitializationException(const std::string& message = "Failed to initialize plugin")
        : BaseException(message) {}
};

/**
 * @brief Thrown when an unknown command is encountered
 */
class PALANTIR_CORE_API UnknownCommandException : public BaseException {
public:
    explicit UnknownCommandException(const std::string& message = "Unknown command") : BaseException(message) {}
};

/**
 * @brief Thrown when UI components are not found
 */
class PALANTIR_CORE_API UIComponentNotFoundException : public BaseException {
public:
    explicit UIComponentNotFoundException(const std::string& message = "UI component not found")
        : BaseException(message) {}
};

}  // namespace palantir::exception