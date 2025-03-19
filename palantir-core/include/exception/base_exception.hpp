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

    BaseException(const BaseException&) = delete;
    auto operator=(const BaseException&) -> BaseException& = delete;
    BaseException(BaseException&&) = delete;
    auto operator=(BaseException&&) -> BaseException& = delete;

    ~BaseException() override = default;
};
#pragma warning(pop)
}  // namespace palantir::exception