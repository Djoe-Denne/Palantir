#include "signal/keyboard_input_signal_factory.hpp"
#include "input/keyboard_input_factory.hpp"
// The implementation of SignalFactory has been moved to the header file
// since it is now a template class.

// This file is kept for compatibility with existing code but doesn't 
// contain any implementation code anymore.

namespace palantir::signal {

auto KeyboardInputSignalFactory::createSignals() const -> std::vector<std::unique_ptr<ISignal>> {
    std::vector<std::unique_ptr<ISignal>> signals;
    for (const auto& commandName : inputFactory_->getConfiguredCommands()) {
        auto command = command::CommandFactory::getInstance()->getCommand(commandName);
        if (command) {
            auto input = inputFactory_->createInput(commandName);
            signals.push_back(
                std::make_unique<InputSignal>(std::move(input), std::move(command), command->useDebounce()));
        } else {
            DebugLog("Unknown command in configuration: {}", commandName);
            throw palantir::exception::TraceableUnknownCommandException("Unknown command in configuration: " +
                                                                        commandName);
        }
    }

    return signals;
}

void KeyboardInputSignalFactory::setInputFactory(std::shared_ptr<input::InputFactory<input::KeyboardInputFactory>> inputFactory) {
    inputFactory_ = inputFactory;
}

}  // namespace palantir::signal