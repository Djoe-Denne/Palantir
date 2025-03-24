#pragma once

#include "config/config.hpp"
#include <filesystem>

namespace palantir::config {

class DesktopConfig : public Config {
public:
    DesktopConfig() = default;
    ~DesktopConfig() override = default;

    DesktopConfig(const DesktopConfig&) = delete;
    auto operator=(const DesktopConfig&) -> DesktopConfig& = delete;

    DesktopConfig(DesktopConfig&&) = delete;
    auto operator=(DesktopConfig&&) -> DesktopConfig& = delete;

    static constexpr const char* CONFIG_SHORTCUTS_PATH = "config/shortcuts.ini";

    [[nodiscard]] virtual auto getShortcutsPath() const -> std::filesystem::path {
        return std::filesystem::path(CONFIG_SHORTCUTS_PATH);
    }
};

} // namespace palantir::config
