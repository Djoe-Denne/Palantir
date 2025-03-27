#pragma once

#include <filesystem>
#include <string>

#include "core_export.hpp"

namespace palantir::config {

class PALANTIR_CORE_API Config {
public:
    virtual ~Config() = default;
    virtual auto getConfigurationFormat() const -> std::string = 0;
    virtual auto getConfigPath() const -> std::filesystem::path = 0;
};

}  // namespace palantir::config
