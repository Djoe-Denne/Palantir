#pragma once

#include "plugin/iplugin.hpp"

namespace interview_cheater::plugins {

class CommandsPlugin : public plugin::IPlugin {
public:
    auto initialize() -> bool override;
    auto shutdown() -> void override;
    [[nodiscard]] auto getName() const -> std::string override;
    [[nodiscard]] auto getVersion() const -> std::string override;
};

} // namespace interview_cheater::plugins

// Register the plugin
IMPLEMENT_PLUGIN(interview_cheater::plugins::CommandsPlugin) 