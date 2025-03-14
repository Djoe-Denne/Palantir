#pragma once

#include "core_export.hpp"

namespace palantir::window::component::message::resize {

/**
 * Value Object for resize messages
 */
struct PALANTIR_CORE_API ResizeMessageVO {
    int width;
    int height;
};

} // namespace palantir::window::component::message::resize 