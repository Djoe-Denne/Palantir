#include "signal/signal_factory.hpp"

#include "command/show_command.hpp"
#include "command/stop_command.hpp"
#include "input/input_factory.hpp"
#include "input/configurable_input.hpp"
#include "signal/signal.hpp"
#include "window/window_manager.hpp"
#include "application.hpp"
#include "platform/application.hpp"
#include "utils/logger.hpp"
#include <stdexcept>

namespace interview_cheater::signal {

auto SignalFactory::createSignals(Application& app) -> std::vector<std::unique_ptr<ISignal>> {
    std::vector<std::unique_ptr<ISignal>> signals;

    for (const auto& commandName : input::InputFactory::getConfiguredCommands()) {
        if (commandName == "toggle") {
            auto command = std::make_unique<command::ShowCommand>(app.getWindowManager());
            auto input = input::InputFactory::createInput(commandName);
            signals.push_back(std::make_unique<Signal>(std::move(input), std::move(command), true));
        }
        else if (commandName == "stop") {
            auto command = std::make_unique<command::StopCommand>(app);
            auto input = input::InputFactory::createInput(commandName);
            signals.push_back(std::make_unique<Signal>(std::move(input), std::move(command), false));
        }
        else {
            DEBUG_LOG("Unknown command in configuration: {}", commandName);
            throw std::runtime_error("Unknown command in configuration: " + commandName);
        }
    }

    return signals;
}

}  // namespace interview_cheater::signal 