#pragma once

#include "plugin_loader/iplugin.hpp"

namespace interview_cheater {
namespace plugins {

class CommandsPlugin : public palantir::plugin::IPlugin {
public:
    bool initialize() override;
    void shutdown() override;
    std::string getName() const override;
    std::string getVersion() const override;
};

} // namespace plugins
} // namespace interview_cheater

// Register the plugin
IMPLEMENT_PLUGIN(interview_cheater::plugins::CommandsPlugin) 