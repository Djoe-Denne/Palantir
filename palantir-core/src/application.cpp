#include "application.hpp"

#include <stdexcept>

#include "input/input_factory.hpp"
#include "signal/signal_factory.hpp"
#include "signal/signal_manager.hpp"
#include "utils/logger.hpp"
#include "window/window_manager.hpp"

namespace interview_cheater {

namespace {
Application* instance_ = nullptr;
}

// Implementation class definition
class Application::ApplicationImpl {
public:
    explicit ApplicationImpl(const std::string& configPath) : configPath_(configPath) {
        DEBUG_LOG("Creating application with config: {}", configPath);
        input::InputFactory::initialize(configPath_);
        DEBUG_LOG("Input configuration initialized");
    }

    auto attachSignals() -> void {
        DEBUG_LOG("Attaching signals from configuration");
        auto signals = signal::SignalFactory::createSignals(*Application::getInstance());
        for (auto& signal : signals) {
            signal::SignalManager::getInstance().addSignal(std::move(signal));
        }
        signal::SignalManager::getInstance().startSignals();
        DEBUG_LOG("Signals attached and started");
    }

private:
    const std::string configPath_;
};

// Static member functions
auto Application::getInstancePtr() -> Application*& { return instance_; }

auto Application::setInstancePtr(Application* instance) -> void { instance_ = instance; }

// Constructor and destructor
Application::Application(const std::string& configPath) : pImpl_(std::make_unique<ApplicationImpl>(configPath)) {}

Application::~Application() = default;

// Public interface implementations
auto Application::getSignalManager() -> signal::SignalManager& { return signal::SignalManager::getInstance(); }

auto Application::getWindowManager() -> window::WindowManager& { return window::WindowManager::getInstance(); }

auto Application::attachSignals() -> void { pImpl_->attachSignals(); }

}  // namespace interview_cheater