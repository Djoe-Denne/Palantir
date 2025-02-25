#include "command/commands_plugin.hpp"
#include "command/command_factory.hpp"
#include "command/show_command.hpp"
#include "command/stop_command.hpp"

namespace interview_cheater {
namespace plugins {

bool CommandsPlugin::initialize() {
    // Register commands
    command::CommandFactory::getInstance().registerCommand("show", []() {
        return std::make_unique<command::ShowCommand>();
    });

    command::CommandFactory::getInstance().registerCommand("stop", []() {
        return std::make_unique<command::StopCommand>();
    });

    return true;
}

void CommandsPlugin::shutdown() {
    // Unregister commands
    command::CommandFactory::getInstance().unregisterCommand("show");
    command::CommandFactory::getInstance().unregisterCommand("stop");
}

std::string CommandsPlugin::getName() const {
    return "Commands Plugin";
}

std::string CommandsPlugin::getVersion() const {
    return "1.0.0";
}

} // namespace plugins
} // namespace interview_cheater 