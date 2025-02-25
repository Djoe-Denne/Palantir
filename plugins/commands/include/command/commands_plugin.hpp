#pragma once

#include "plugin/iplugin.hpp"

namespace interview_cheater::plugins {

class CommandsPlugin : public plugin::IPlugin {
public:
    bool initialize() override;
    void shutdown() override;
    std::string getName() const override;
    std::string getVersion() const override;
};

} // namespace interview_cheater::plugins

// Register the plugin
IMPLEMENT_PLUGIN(interview_cheater::plugins::CommandsPlugin) 