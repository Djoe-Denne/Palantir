#include "signal/signal_factory.hpp"

#include <stdexcept>

#include "application.hpp"
#include "command/command_factory.hpp"
#include "command/icommand.hpp"
#include "config/config.hpp"
#include "exception/exceptions.hpp"
#include "input/keyboard_input.hpp"
#include "input/input_factory.hpp"
#include "input/keyboard_input_factory.hpp"
#include "signal/signal.hpp"
#include "utils/logger.hpp"
#include "window/window_manager.hpp"

namespace palantir::signal {

class SignalFactory::SignalFactoryImpl {
public:
    SignalFactoryImpl(const config::Config& config) : inputFactory_(std::make_unique<input::InputFactory<input::KeyboardInputFactory>>(config)) {}
    ~SignalFactoryImpl() = default;

    SignalFactoryImpl(const SignalFactoryImpl&) = delete;
    auto operator=(const SignalFactoryImpl&) -> SignalFactoryImpl& = delete;
    SignalFactoryImpl(SignalFactoryImpl&&) = delete;
    auto operator=(SignalFactoryImpl&&) -> SignalFactoryImpl& = delete;

    auto createSignals() const -> std::vector<std::unique_ptr<ISignal>> {
        std::vector<std::unique_ptr<ISignal>> signals;
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
    std::unique_ptr<input::InputFactory<input::KeyboardInputFactory>> inputFactory_;
};

SignalFactory::SignalFactory(const config::Config& config) : pimpl_(std::make_unique<SignalFactoryImpl>(config)) {}

SignalFactory::~SignalFactory() = default;

auto SignalFactory::createSignals() const -> std::vector<std::unique_ptr<ISignal>> { return pimpl_->createSignals(); }

}  // namespace palantir::signal