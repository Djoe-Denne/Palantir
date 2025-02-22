#include "application.hpp"

#include "input/input_factory.hpp"
#include "platform_application.hpp"
#include "signal/signal_factory.hpp"
#include "utils/logger.hpp"

namespace interview_cheater {

Application* Application::instance_ = nullptr;

auto Application::getInstance(const std::string& configPath) -> Application* {
    if (instance_ == nullptr) {
#ifdef _WIN32
        instance_ = new PlatformApplication(configPath);
#elif defined(__APPLE__)
        instance_ = new PlatformApplication(configPath);
#endif
    }
    return instance_;
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