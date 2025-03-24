#pragma once

#include <filesystem>
#include "core_export.hpp"

namespace palantir::config {

class PALANTIR_CORE_API Config {
public:
    virtual ~Config() = default;
    [[nodiscard]] virtual auto getShortcutsPath() const -> std::filesystem::path = 0;
};

} // namespace palantir::config