#include "application.hpp"

#include <stdexcept>

#include "input/input_factory.hpp"
#include "signal/signal_factory.hpp"
#include "signal/keyboard_input_signal_factory.hpp"
#include "signal/signal_manager.hpp"
#include "utils/logger.hpp"
#include "window/window_manager.hpp"

namespace palantir {

// Initialize static instance
std::shared_ptr<Application> Application::instance_ = nullptr;

// Implementation class definition
class Application::ApplicationImpl {
public:
    explicit ApplicationImpl(const std::string& configPath) : configPath_(configPath) {
        DebugLog("Creating application with config: {}", configPath);
        input::InputFactory::getInstance()->initialize(configPath_);
        DebugLog("Input configuration initialized");
    }

    auto attachSignals() const -> void {
        DebugLog("Attaching signals from configuration");
        auto app = Application::getInstance();
        auto signals = signal::SignalFactory<signal::KeyboardInputSignalFactory>::getInstance()->createSignals();
        for (auto& signal : signals) {
            signal::SignalManager::getInstance()->addSignal(std::move(signal));
        }
        signal::SignalManager::getInstance()->startSignals();
        DebugLog("Signals attached and started");
    }

private:
    const std::string configPath_;
};

auto Application::getInstance() -> std::shared_ptr<Application> { return instance_; }

// Static member functions
auto Application::setInstance(const std::shared_ptr<Application>& instance) -> void { instance_ = instance; }

// Constructor and destructor
Application::Application(const std::string& configPath) : pImpl_(std::make_unique<ApplicationImpl>(configPath)) {}

Application::Application() = default;

Application::~Application() = default;

// Public interface implementations
auto Application::getSignalManager() const -> std::shared_ptr<signal::SignalManager> {
    return signal::SignalManager::getInstance();
}

auto Application::getWindowManager() const -> std::shared_ptr<window::WindowManager> {
    return window::WindowManager::getInstance();
}

auto Application::attachSignals() -> void { pImpl_->attachSignals(); }

}  // namespace palantir