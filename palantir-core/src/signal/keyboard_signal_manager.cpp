#include "signal/keyboard_signal_manager.hpp"

#include <vector>

#include "signal/isignal.hpp"
#include "signal/keyboard_api.hpp"
#include "signal/keyboard_signal_manager_impl.hpp"
#include "utils/logger.hpp"

namespace palantir::signal {

KeyboardSignalManager::KeyboardSignalManager()
    : pImpl_(std::make_unique<KeyboardSignalManagerImpl>(std::make_unique<KeyboardApi>())) {}

KeyboardSignalManager::KeyboardSignalManager(std::unique_ptr<KeyboardApi> keyboardApi)
    : pImpl_(std::make_unique<KeyboardSignalManagerImpl>(std::move(keyboardApi))) {}

KeyboardSignalManager::~KeyboardSignalManager() = default;

auto KeyboardSignalManager::addSignal(std::unique_ptr<ISignal> signal) -> void {
    DebugLog("Adding signal to manager");
    pImpl_->addSignal(std::move(signal));
}

auto KeyboardSignalManager::startSignals() const -> void {
    DebugLog("Starting signals");
    pImpl_->startSignals();
}

auto KeyboardSignalManager::stopSignals() const -> void {
    DebugLog("Stopping signals");
    pImpl_->stopSignals();
}

auto KeyboardSignalManager::checkSignals(const std::any& event) const -> void { pImpl_->checkSignals(event); }

}  // namespace palantir::signal