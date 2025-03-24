#pragma once

#include <filesystem>
#include <string_view>
#include "core_export.hpp"

namespace palantir::config {

constexpr const char* CONFIG_SHORTCUTS_PATH = "config/shortcuts.ini";
// Alternative options:
// constexpr std::string_view CONFIG_SHORTCUTS_PATH = "config/shortcuts.ini";
// inline const std::filesystem::path CONFIG_SHORTCUTS_PATH = "config/shortcuts.ini";

} // namespace palantir::config
