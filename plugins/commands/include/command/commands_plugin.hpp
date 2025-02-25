#pragma once

#include "plugin/iplugin.hpp"

#ifdef _WIN32
    #ifdef COMMANDS_PLUGIN_EXPORTS
        #define COMMANDS_PLUGIN_API __declspec(dllexport)
    #else
        #define COMMANDS_PLUGIN_API __declspec(dllimport)
    #endif
#else
    #define COMMANDS_PLUGIN_API
#endif

namespace interview_cheater::plugins {

class COMMANDS_PLUGIN_API CommandsPlugin : public plugin::IPlugin {
public:
    auto initialize() -> bool override;
    auto shutdown() -> void override;
    [[nodiscard]] auto getName() const -> std::string override;
    [[nodiscard]] auto getVersion() const -> std::string override;
};

} // namespace interview_cheater::plugins

// Register the plugin
IMPLEMENT_PLUGIN(interview_cheater::plugins::CommandsPlugin) 