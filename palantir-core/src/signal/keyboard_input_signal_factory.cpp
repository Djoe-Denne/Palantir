#include "signal/keyboard_input_signal_factory.hpp"

#include "signal/signal.hpp"
#include "input/input_factory.hpp"
#include "input/keyboard_input_factory.hpp"
#include "command/command_factory.hpp"
#include "exception/exceptions.hpp"
#include "utils/logger.hpp"

namespace palantir::signal {

KeyboardInputSignalFactory::KeyboardInputSignalFactory(const config::Config& config) 
    : inputFactory_(std::make_unique<input::InputFactory<input::KeyboardInputFactory>>(config)) {
}

KeyboardInputSignalFactory::KeyboardInputSignalFactory(std::unique_ptr<input::InputFactory<input::KeyboardInputFactory>> inputFactory)
    : inputFactory_(std::move(inputFactory)) {
}

auto KeyboardInputSignalFactory::createSignals() const -> std::vector<std::unique_ptr<ISignal>> {
    std::vector<std::unique_ptr<ISignal>> signals;
    
    // Get all configured commands from the input factory
    for (const auto& commandName : inputFactory_->getConfiguredCommands()) {
        // Get the command
        auto command = command::CommandFactory::getInstance()->getCommand(commandName);
        if (command) {
            // Create input for the command
            auto input = inputFactory_->createInput(commandName);
            
            // Create signal connecting input to command
            signals.push_back(
                std::make_unique<Signal>(std::move(input), std::move(command), command->useDebounce()));
        } else {
            // Log and throw if command not found
            DebugLog("Unknown command in configuration: {}", commandName);
            throw palantir::exception::TraceableUnknownCommandException("Unknown command in configuration: " +
                                                                        commandName);
        }
    }
    
    return signals;
}

}  // namespace palantir::signal