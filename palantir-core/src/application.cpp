#include "application.hpp"

#include <stdexcept>

#include "signal/keyboard_signal_factory.hpp"
#include "signal/keyboard_signal_manager.hpp"
#include "utils/logger.hpp"
#include "window/window_manager.hpp"

namespace palantir {

// Initialize static instance
std::shared_ptr<Application> Application::instance_ = nullptr;

// Implementation class definition
class Application::ApplicationImpl {
public:
    explicit ApplicationImpl() {
        DebugLog("Creating application");
        signalFactory_ = std::make_shared<signal::KeyboardSignalFactory>();
        signalManager_ = std::make_shared<signal::KeyboardSignalManager>();
    }

    auto attachSignals() const -> void {
        DebugLog("Attaching signals from configuration");
        auto app = Application::getInstance();
        auto signals = signalFactory_->createSignals();
        for (auto& signal : signals) {
            signalManager_->addSignal(std::move(signal));
        }
        signalManager_->startSignals();
        DebugLog("Signals attached and started");
    }

    [[nodiscard]] auto getSignalManager() const -> const std::shared_ptr<signal::ISignalManager>& {
        return signalManager_;
    }

private:
    std::shared_ptr<signal::ISignalFactory> signalFactory_;
    std::shared_ptr<signal::ISignalManager> signalManager_;
};

auto Application::getInstance() -> std::shared_ptr<Application> { return instance_; }

// Static member functions
auto Application::setInstance(const std::shared_ptr<Application>& instance) -> void { instance_ = instance; }

// Constructor and destructor
Application::Application() : pImpl_(std::make_unique<ApplicationImpl>()) {}

Application::~Application() = default;

// Public interface implementations
auto Application::getSignalManager() const -> const std::shared_ptr<signal::ISignalManager>& {
    return pImpl_->getSignalManager();
}

auto Application::getWindowManager() const -> const std::shared_ptr<window::WindowManager>& {
    return window::WindowManager::getInstance();
}

auto Application::attachSignals() -> void { pImpl_->attachSignals(); }

}  // namespace palantir