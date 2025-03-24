#pragma once

#include <filesystem>
#include "core_export.hpp"

namespace palantir::config {

class PALANTIR_CORE_API Config {
public:
    [[nodiscard]] virtual auto getShortcutsPath() -> std::filesystem::path = 0;
};

} // namespace palantir::config