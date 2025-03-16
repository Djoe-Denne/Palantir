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

namespace palantir::signal {

std::shared_ptr<SignalFactory> SignalFactory::instance_;

class SignalFactory::SignalFactoryImpl {
public:
    SignalFactoryImpl() = default;
    ~SignalFactoryImpl() = default;

    SignalFactoryImpl(const SignalFactoryImpl&) = delete;
    auto operator=(const SignalFactoryImpl&) -> SignalFactoryImpl& = delete;
    SignalFactoryImpl(SignalFactoryImpl&&) = delete;
    auto operator=(SignalFactoryImpl&&) -> SignalFactoryImpl& = delete;

    auto createSignals(const std::shared_ptr<Application>& app) -> std::vector<std::unique_ptr<ISignal>> {
        std::vector<std::unique_ptr<ISignal>> signals;

        for (const auto& commandName : input::InputFactory::getInstance()->getConfiguredCommands()) {
            auto command = command::CommandFactory::getInstance()->getCommand(commandName);
            if (command) {
                auto input = input::InputFactory::getInstance()->createInput(commandName);
                signals.push_back(
                    std::make_unique<Signal>(std::move(input), std::move(command), command->useDebounce()));
            } else {
                DebugLog("Unknown command in configuration: {}", commandName);
                throw std::runtime_error("Unknown command in configuration: " + commandName);
            }
        }

        return signals;
    }
};

SignalFactory::SignalFactory() : pimpl_(std::make_unique<SignalFactoryImpl>()) {}  // NOLINT

SignalFactory::~SignalFactory() = default;

auto SignalFactory::getInstance() -> std::shared_ptr<SignalFactory> {
    if (!instance_) {
        instance_ = std::shared_ptr<SignalFactory>(new SignalFactory());
    }
    return instance_;
}

auto SignalFactory::setInstance(const std::shared_ptr<SignalFactory>& instance) -> void { instance_ = instance; }

auto SignalFactory::createSignals(const std::shared_ptr<Application>& app) -> std::vector<std::unique_ptr<ISignal>> {
    return pimpl_->createSignals(app);
}

}  // namespace palantir::signal