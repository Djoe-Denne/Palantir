#include "signal/keyboard_signal_factory.hpp"

#include <stdexcept>

#include "command/command_factory.hpp"
#include "command/icommand.hpp"
#include "config/config.hpp"
#include "config/desktop_config.hpp"
#include "exception/exceptions.hpp"
#include "input/keyboard_input.hpp"
#include "input/keyboard_input_factory.hpp"
#include "signal/signal.hpp"
#include "utils/logger.hpp"

namespace palantir::signal {

class KeyboardSignalFactory::KeyboardSignalFactoryImpl {
public:
    KeyboardSignalFactoryImpl()
        : inputFactory_(std::make_shared<input::KeyboardInputFactory>(std::make_shared<config::DesktopConfig>())) {}
    KeyboardSignalFactoryImpl(const std::shared_ptr<input::IInputFactory>& inputFactory)
        : inputFactory_(inputFactory) {}
    ~KeyboardSignalFactoryImpl() = default;

    KeyboardSignalFactoryImpl(const KeyboardSignalFactoryImpl&) = delete;
    auto operator=(const KeyboardSignalFactoryImpl&) -> KeyboardSignalFactoryImpl& = delete;
    KeyboardSignalFactoryImpl(KeyboardSignalFactoryImpl&&) = delete;
    auto operator=(KeyboardSignalFactoryImpl&&) -> KeyboardSignalFactoryImpl& = delete;

    auto createSignals() const -> std::vector<std::unique_ptr<ISignal>> {
        std::vector<std::unique_ptr<ISignal>> signals;
        inputFactory_->initialize();
        for (const auto& commandName : inputFactory_->getConfiguredCommands()) {
            auto command = command::CommandFactory::getInstance()->getCommand(commandName);
            if (command) {
                auto input = inputFactory_->createInput(commandName);
                signals.push_back(
                    std::make_unique<Signal>(std::move(input), std::move(command), command->useDebounce()));
            } else {
                DebugLog("Unknown command in configuration: {}", commandName);
                throw palantir::exception::TraceableUnknownCommandException("Unknown command in configuration: " +
                                                                            commandName);
            }
        }

        return signals;
    }

private:
    std::shared_ptr<input::IInputFactory> inputFactory_;
};

KeyboardSignalFactory::KeyboardSignalFactory() : pimpl_(std::make_unique<KeyboardSignalFactoryImpl>()) {}  // NOLINT
KeyboardSignalFactory::KeyboardSignalFactory(const std::shared_ptr<input::IInputFactory>& inputFactory)
    : pimpl_(std::make_unique<KeyboardSignalFactoryImpl>(inputFactory)) {}

KeyboardSignalFactory::~KeyboardSignalFactory() = default;

auto KeyboardSignalFactory::createSignals() const -> std::vector<std::unique_ptr<ISignal>> {
    return pimpl_->createSignals();
}

}  // namespace palantir::signal