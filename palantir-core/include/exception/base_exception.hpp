#pragma once

#include <stdexcept>
#include <string>

#include "core_export.hpp"

namespace palantir::exception {

/**
 * @brief Base class for all custom palantir exceptions
 */
#pragma warning(push)
#pragma warning(disable : 4251)
class PALANTIR_CORE_API BaseException : public std::runtime_error {
public:
    explicit BaseException(const std::string& message) : std::runtime_error(message) {}
    virtual ~BaseException() = default;
};
#pragma warning(pop)
}  // namespace palantir::exception