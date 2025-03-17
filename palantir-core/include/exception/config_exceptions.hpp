#pragma once

#include "base_exception.hpp"

namespace palantir::exception {

/**
 * @brief Thrown when configuration file operations fail
 */
class PALANTIR_CORE_API ConfigFileException : public BaseException {
public:
    explicit ConfigFileException(const std::string& message = "Configuration file error") : BaseException(message) {}
};

/**
 * @brief Thrown when content management operations fail
 */
class PALANTIR_CORE_API ContentManagerException : public BaseException {
public:
    explicit ContentManagerException(const std::string& message = "Content manager error") : BaseException(message) {}
};

/**
 * @brief Thrown when loading a required resource fails
 */
class PALANTIR_CORE_API ResourceLoadingException : public BaseException {
public:
    explicit ResourceLoadingException(const std::string& message = "Failed to load resource")
        : BaseException(message) {}
};

}  // namespace palantir::exception