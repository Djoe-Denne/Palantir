#include "signal/keyboard_signal_manager.hpp"

#include <vector>

#include "signal/isignal.hpp"
#include "signal/keyboard_api.hpp"
#include "signal/keyboard_signal_manager_impl.hpp"
#include "signal/keyboard_signal_factory.hpp"
#include "utils/logger.hpp"

namespace palantir::signal {

KeyboardSignalManager::KeyboardSignalManager()
    : pImpl_(std::make_unique<KeyboardSignalManagerImpl>(std::make_unique<KeyboardApi>())),
      factory_(std::make_shared<KeyboardSignalFactory>()) {}

KeyboardSignalManager::KeyboardSignalManager(std::unique_ptr<KeyboardApi> keyboardApi)
    : pImpl_(std::make_unique<KeyboardSignalManagerImpl>(std::move(keyboardApi))),
      factory_(std::make_shared<KeyboardSignalFactory>()) {}

KeyboardSignalManager::KeyboardSignalManager(const std::shared_ptr<ISignalFactory>& factory)
    : pImpl_(std::make_unique<KeyboardSignalManagerImpl>(std::make_unique<KeyboardApi>())),
      factory_(factory) {}

KeyboardSignalManager::KeyboardSignalManager(const std::shared_ptr<ISignalFactory>& factory, std::unique_ptr<KeyboardApi> keyboardApi)
    : pImpl_(std::make_unique<KeyboardSignalManagerImpl>(std::move(keyboardApi))),
      factory_(factory) {}

KeyboardSignalManager::~KeyboardSignalManager() = default;

auto KeyboardSignalManager::startSignals() const -> void {
    DebugLog("Starting signals");
    if (!pImpl_->hasSignals() && factory_) {
        DebugLog("No signals present, creating signals from factory");
        auto signals = factory_->createSignals();
        for (auto& signal : signals) {
            pImpl_->addSignal(std::move(signal));
        }
    }
    pImpl_->startSignals();
}

auto KeyboardSignalManager::stopSignals() const -> void {
    DebugLog("Stopping signals");
    pImpl_->stopSignals();
}

auto KeyboardSignalManager::checkSignals(const std::any& event) const -> void { pImpl_->checkSignals(event); }

}  // namespace palantir::signal