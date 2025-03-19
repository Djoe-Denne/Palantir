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
    using std::runtime_error::runtime_error;

    BaseException(const BaseException &) = default;
    auto operator=(const BaseException &) -> BaseException & = default;
    BaseException(BaseException &&) = default;
    auto operator=(BaseException &&) -> BaseException & = default;

    ~BaseException() override = default;
};
#pragma warning(pop)
}  // namespace palantir::exception