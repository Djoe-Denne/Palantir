#include "signal/keyboard_signal_manager.hpp"

#include <Windows.h>
#include <vector>

#include "signal/isignal.hpp"
#include "utils/logger.hpp"

namespace palantir::signal {


/**
 * @brief Windows-specific implementation of SignalManager
 *
 * This class handles the Windows-specific keyboard hook setup and management.
 * It uses the Windows Low Level Keyboard Hook to capture keyboard events
 * globally, even when the application is not in focus.
 */
class KeyboardSignalManager::KeyboardSignalManagerImpl {
public:
    /**
     * @brief Construct the implementation
     * @param parent Pointer to the owning SignalManager instance
     */
    KeyboardSignalManagerImpl() {
        DebugLog("Initializing SignalManager implementation");
        hook_ = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, GetModuleHandle(nullptr), 0);
        if (hook_ == nullptr) {
            DebugLog("Failed to set keyboard hook");
        }
        instance_ = this;
    }

    KeyboardSignalManagerImpl(const KeyboardSignalManagerImpl&) = delete;
    auto operator=(const KeyboardSignalManagerImpl&) -> KeyboardSignalManagerImpl& = delete;
    KeyboardSignalManagerImpl(KeyboardSignalManagerImpl&&) = delete;
    auto operator=(KeyboardSignalManagerImpl&&) -> KeyboardSignalManagerImpl& = delete;

    /**
     * @brief Destroy the implementation
     */
    ~KeyboardSignalManagerImpl() {
        if (hook_ != nullptr) {
            UnhookWindowsHookEx(hook_);
            hook_ = nullptr;
        }
    }

    /**
     * @brief Add a signal to the collection
     */
    auto addSignal(std::unique_ptr<ISignal> signal) -> void {
        signals_.push_back(std::move(signal));
    }

    /**
     * @brief Start all signals
     */
    auto startSignals() const -> void {
        for (const auto& signal : signals_) {
            signal->start();
        }
    }

    /**
     * @brief Stop all signals
     */
    auto stopSignals() const -> void {
        for (const auto& signal : signals_) {
            signal->stop();
        }
    }

    /**
     * @brief Check all signals
     */
    auto checkSignals(const std::any& event) const -> void {
        for (const auto& signal : signals_) {
            signal->check(event);
        }
    }

private:
    /**
     * @brief Windows keyboard hook callback
     */
    static auto CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) -> LRESULT {
        if (nCode == HC_ACTION && wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN || wParam == WM_KEYUP || wParam == WM_SYSKEYUP) {
            instance_->checkSignals(nullptr);
        }
        return CallNextHookEx(nullptr, nCode, wParam, lParam);
    }

    HHOOK hook_{nullptr};                                  ///< Windows keyboard hook handle
    std::vector<std::unique_ptr<ISignal>> signals_;       ///< Collection of managed signals
    static KeyboardSignalManagerImpl* instance_;          ///< Singleton instance
};



KeyboardSignalManager::KeyboardSignalManagerImpl* KeyboardSignalManager::KeyboardSignalManagerImpl::instance_ = nullptr;

KeyboardSignalManager::KeyboardSignalManager() : pImpl_(std::make_unique<KeyboardSignalManagerImpl>()) {
}

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

auto KeyboardSignalManager::checkSignals(const std::any& event) const -> void {
    pImpl_->checkSignals(event);
}

}  // namespace palantir::signal