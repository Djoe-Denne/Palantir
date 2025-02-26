#include "command/commands_plugin.hpp"
#include "command/command_factory.hpp"
#include "command/show_command.hpp"
#include "command/stop_command.hpp"
#include "command/window_screenshot_command.hpp"

namespace interview_cheater::plugins {

CommandsPlugin::~CommandsPlugin() {
    // Call shutdown directly to avoid virtual dispatch during destruction
    CommandsPlugin::shutdown();
}

// Static functions to create commands
static auto createShowCommand() -> std::unique_ptr<command::ICommand> {
    return std::make_unique<command::ShowCommand>();
}

static auto createStopCommand() -> std::unique_ptr<command::ICommand> {
    return std::make_unique<command::StopCommand>();
}

static auto createWindowScreenshotCommand() -> std::unique_ptr<command::ICommand> {
    return std::make_unique<command::WindowScreenshotCommand>();
}

auto CommandsPlugin::initialize() -> bool {
    // Register commands using function pointers
    command::CommandFactory::getInstance().registerCommand("toggle", &createShowCommand);
    command::CommandFactory::getInstance().registerCommand("stop", &createStopCommand);
    command::CommandFactory::getInstance().registerCommand("window-screenshot", &createWindowScreenshotCommand);
    return true;
}

auto CommandsPlugin::shutdown() -> void {
    // Unregister commands
    command::CommandFactory::getInstance().unregisterCommand("toggle");
    command::CommandFactory::getInstance().unregisterCommand("stop");
    command::CommandFactory::getInstance().unregisterCommand("window-screenshot");
}

auto CommandsPlugin::getName() const -> std::string {
    return "Commands Plugin";
}

auto CommandsPlugin::getVersion() const -> std::string {
    return "1.0.0";
}

} // namespace interview_cheater::plugins 