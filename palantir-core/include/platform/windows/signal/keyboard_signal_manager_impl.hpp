#include <vector>

#include "signal/isignal.hpp"
#include "signal/keyboard_api.hpp"
#include "signal/keyboard_signal_manager.hpp"
#include "utils/logger.hpp"

namespace palantir::signal {

/**
 * @brief Platform-specific implementation of SignalManager
 *
 * This class handles the platform-specific keyboard hook setup and management.
 * It uses low-level keyboard hooks to capture keyboard events
 * globally, even when the application is not in focus.
 */
class KeyboardSignalManager::KeyboardSignalManagerImpl {
public:
    /**
     * @brief Construct the implementation
     * @param keyboardApi Pointer to Keyboard API wrapper (defaults to a new instance)
     */
    explicit KeyboardSignalManagerImpl(std::unique_ptr<KeyboardApi> keyboardApi)
        : keyboardApi_(std::move(keyboardApi)) {
        if (!keyboardApi_) {
            keyboardApi_ = std::make_unique<KeyboardApi>();
        }

        DebugLog("Initializing SignalManager implementation for Windows");
        hook_ =
            keyboardApi_->SetHook(WH_KEYBOARD_LL, LowLevelKeyboardProc, keyboardApi_->GetModuleOSHandle(nullptr), 0);
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
            keyboardApi_->UnhookKeyboard(hook_);
            hook_ = nullptr;
        }
    }

    /**
     * @brief Add a signal to the collection
     */
    auto addSignal(std::unique_ptr<ISignal> signal) -> void { signals_.push_back(std::move(signal)); }

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
        if (nCode == HC_ACTION &&
            (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN || wParam == WM_KEYUP || wParam == WM_SYSKEYUP)) {
            instance_->checkSignals(nullptr);
        }
        return instance_->keyboardApi_->CallNextHook(nullptr, nCode, wParam, lParam);
    }

    using HookHandleType = KeyboardHookTypes::HookHandle;

    /// Platform-specific hook handle
    HookHandleType hook_{nullptr};
    /// Collection of managed signals
    std::vector<std::unique_ptr<ISignal>> signals_;
    /// Platform-specific keyboard API implementation
    std::unique_ptr<KeyboardApi> keyboardApi_;
    /// Singleton instance for callback access
    static KeyboardSignalManagerImpl* instance_;
};

KeyboardSignalManager::KeyboardSignalManagerImpl* KeyboardSignalManager::KeyboardSignalManagerImpl::instance_ = nullptr;

}  // namespace palantir::signal