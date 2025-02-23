#include "application.hpp"

#include "input/input_factory.hpp"
#include "signal/signal_factory.hpp"
#include "utils/logger.hpp"
#include <stdexcept>

namespace interview_cheater {

namespace {
Application* instance_ = nullptr;
}

auto Application::getInstancePtr() -> Application*& {
    return instance_;
}

auto Application::setInstancePtr(Application* instance) -> void {
    instance_ = instance;
}

Application::Application(const std::string& configPath) : configPath_(configPath) {
    DEBUG_LOG("Creating application with config: {}", configPath);

    // Initialize input configuration
    input::InputFactory::initialize(configPath_);
    DEBUG_LOG("Input configuration initialized");
}

auto Application::attachSignals() -> void {
    DEBUG_LOG("Attaching signals from configuration");

    auto signals = signal::SignalFactory::createSignals(*this);
    for (auto& signal : signals) {
        signalManager_.addSignal(std::move(signal));
    }

    signalManager_.startSignals();
    DEBUG_LOG("Signals attached and started");
}

}  // namespace interview_cheater