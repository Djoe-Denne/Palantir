#pragma once

#include <string>

#include "core_export.hpp"

namespace palantir::window::component::message {

/**
 * Value Object for log messages
 */
struct PALANTIR_CORE_API LogMessageVO {
#pragma warning(push)
#pragma warning(disable : 4251)
    std::string message;
#pragma warning(pop)
};

}  // namespace palantir::window::component::message