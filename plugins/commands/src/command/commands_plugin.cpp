#include "command/commands_plugin.hpp"
#include "command/command_factory.hpp"
#include "command/show_command.hpp"
#include "command/stop_command.hpp"
#include "command/window_screenshot_command.hpp"
#include "command/toggle_transparency_command.hpp"
#include "command/toggle_window_anonymity_command.hpp"
#include "command/clear_screenshot_command.hpp"
#include "command/send_sauron_request_command.hpp"
#include "prompt/prompts.hpp"

namespace palantir::plugins {

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

static auto createToggleTransparencyCommand() -> std::unique_ptr<command::ICommand> {
    return std::make_unique<command::ToggleTransparencyCommand>();
}

static auto createToggleWindowAnonymityCommand() -> std::unique_ptr<command::ICommand> {
    return std::make_unique<command::ToggleWindowAnonymityCommand>();
}

static auto createClearScreenshotCommand() -> std::unique_ptr<command::ICommand> {
    return std::make_unique<command::ClearScreenshotCommand>();
}

static auto createSendSauronImplementRequestCommand() -> std::unique_ptr<command::ICommand> {
    return std::make_unique<command::SendSauronRequestCommand>(prompt::SAURON_IMPLEM_REQUEST_PROMPT);
}

static auto createSendSauronFixErrorsRequestCommand() -> std::unique_ptr<command::ICommand> {
    return std::make_unique<command::SendSauronRequestCommand>(prompt::SAURON_FIX_ERRORS_REQUEST_PROMPT);
}

static auto createSendSauronValidateWithTestsRequestCommand() -> std::unique_ptr<command::ICommand> {
    return std::make_unique<command::SendSauronRequestCommand>(prompt::SAURON_VALIDATE_WITH_TESTS_REQUEST_PROMPT);
}

static auto createSendSauronFixTestFailuresRequestCommand() -> std::unique_ptr<command::ICommand> {
    return std::make_unique<command::SendSauronRequestCommand>(prompt::SAURON_FIX_TEST_FAILURES_REQUEST_PROMPT);
}

static auto createSendSauronHandleTodosRequestCommand() -> std::unique_ptr<command::ICommand> {
    return std::make_unique<command::SendSauronRequestCommand>(prompt::SAURON_HANDLE_TODOS_REQUEST_PROMPT);
}

auto CommandsPlugin::initialize() -> bool {
    // Register commands using function pointers
    command::CommandFactory::getInstance()->registerCommand("toggle", &createShowCommand);
    command::CommandFactory::getInstance()->registerCommand("stop", &createStopCommand);
    command::CommandFactory::getInstance()->registerCommand("window-screenshot", &createWindowScreenshotCommand);
    command::CommandFactory::getInstance()->registerCommand("toggle-transparency", &createToggleTransparencyCommand);
    command::CommandFactory::getInstance()->registerCommand("toggle-window-anonymity", &createToggleWindowAnonymityCommand);
    command::CommandFactory::getInstance()->registerCommand("clear-screenshot", &createClearScreenshotCommand);
    command::CommandFactory::getInstance()->registerCommand("send-sauron-implement-request", &createSendSauronImplementRequestCommand);
    command::CommandFactory::getInstance()->registerCommand("send-sauron-fix-errors-request", &createSendSauronFixErrorsRequestCommand);
    command::CommandFactory::getInstance()->registerCommand("send-sauron-validate-with-tests-request", &createSendSauronValidateWithTestsRequestCommand);
    command::CommandFactory::getInstance()->registerCommand("send-sauron-fix-test-failures-request", &createSendSauronFixTestFailuresRequestCommand);
    command::CommandFactory::getInstance()->registerCommand("send-sauron-handle-todos-request", &createSendSauronHandleTodosRequestCommand);
    return true;
}

auto CommandsPlugin::shutdown() -> void {
    // Unregister commands
    command::CommandFactory::getInstance()->unregisterCommand("toggle");
    command::CommandFactory::getInstance()->unregisterCommand("stop");
    command::CommandFactory::getInstance()->unregisterCommand("window-screenshot");
    command::CommandFactory::getInstance()->unregisterCommand("toggle-transparency");
    command::CommandFactory::getInstance()->unregisterCommand("toggle-window-anonymity");
    command::CommandFactory::getInstance()->unregisterCommand("clear-screenshot");
    command::CommandFactory::getInstance()->unregisterCommand("send-sauron-implement-request");
    command::CommandFactory::getInstance()->unregisterCommand("send-sauron-fix-errors-request");
    command::CommandFactory::getInstance()->unregisterCommand("send-sauron-validate-with-tests-request");
    command::CommandFactory::getInstance()->unregisterCommand("send-sauron-fix-test-failures-request");
    command::CommandFactory::getInstance()->unregisterCommand("send-sauron-handle-todos-request");
}

auto CommandsPlugin::getName() const -> std::string {
    return "Commands Plugin";
}

auto CommandsPlugin::getVersion() const -> std::string {
    return "1.0.0";
}

} // namespace palantir::plugins 