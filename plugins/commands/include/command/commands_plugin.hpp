#pragma once

#include "plugin/iplugin.hpp"
#include "plugin_export.hpp"

namespace interview_cheater::plugins {

class COMMANDS_PLUGIN_API CommandsPlugin : public plugin::IPlugin {
public:
    CommandsPlugin() = default;
    ~CommandsPlugin() override;
    
    // Rule of 5
    CommandsPlugin(const CommandsPlugin&) = delete;
    CommandsPlugin& operator=(const CommandsPlugin&) = delete;
    CommandsPlugin(CommandsPlugin&&) = delete;
    CommandsPlugin& operator=(CommandsPlugin&&) = delete;

    auto initialize() -> bool override;
    auto shutdown() -> void override;
    [[nodiscard]] auto getName() const -> std::string override;
    [[nodiscard]] auto getVersion() const -> std::string override;
};

} // namespace interview_cheater::plugins

// Register the plugin
IMPLEMENT_PLUGIN(interview_cheater::plugins::CommandsPlugin) 