#include "signal/signal_factory.hpp"

#include <stdexcept>

#include "application.hpp"
#include "command/command_factory.hpp"
#include "command/icommand.hpp"
#include "input/configurable_input.hpp"
#include "input/input_factory.hpp"
#include "signal/signal.hpp"
#include "utils/logger.hpp"
#include "window/window_manager.hpp"

namespace interview_cheater::signal {

auto SignalFactory::createSignals(Application& app) -> std::vector<std::unique_ptr<ISignal>> {
    std::vector<std::unique_ptr<ISignal>> signals;

    for (const auto& commandName : input::InputFactory::getConfiguredCommands()) {
        auto command = command::CommandFactory::getInstance().getCommand(commandName);
        if (command) {
            auto input = input::InputFactory::createInput(commandName);
            signals.push_back(std::make_unique<Signal>(std::move(input), std::move(command), command->useDebounce()));
        } else {
            DEBUG_LOG("Unknown command in configuration: {}", commandName);
            throw std::runtime_error("Unknown command in configuration: " + commandName);
        }
    }

    return signals;
}

}  // namespace interview_cheater::signal