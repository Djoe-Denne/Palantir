#pragma once

#include "config/config.hpp"
#include <filesystem>

namespace palantir::config {

class DesktopConfig : public Config {
public:
    static constexpr const char* CONFIG_SHORTCUTS_PATH = "config/shortcuts.ini";

    [[nodiscard]] virtual auto getShortcutsPath() -> std::filesystem::path {
        return std::filesystem::path(CONFIG_SHORTCUTS_PATH);
    }
};

} // namespace palantir::config
