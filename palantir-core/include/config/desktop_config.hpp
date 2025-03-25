#pragma once

#include <filesystem>
#include <string>

#include "config/config.hpp"
#include "core_export.hpp"

namespace palantir::config {

class PALANTIR_CORE_API DesktopConfig : public Config {
public:
    static constexpr const char* CONFIG_PATH = "config";
    static constexpr const char* CONFIG_FORMAT = "ini";

    ~DesktopConfig() override = default;

    [[nodiscard]] auto getConfigPath() const -> std::filesystem::path override {
        return std::filesystem::path(CONFIG_PATH);
    }

    [[nodiscard]] auto getConfigurationFormat() const -> std::string override { return CONFIG_FORMAT; }
};

}  // namespace palantir::config
