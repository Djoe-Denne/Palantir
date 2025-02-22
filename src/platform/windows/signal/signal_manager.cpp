#include "signal/signal_manager.hpp"

#include <windows.h>

#include "signal/isignal.hpp"
#include "utils/logger.hpp"

namespace interview_cheater::signal {

/**
 * @brief Windows-specific implementation of SignalManager
 *
 * This class handles the Windows-specific keyboard hook setup and management.
 * It uses the Windows Low Level Keyboard Hook to capture keyboard events
 * globally, even when the application is not in focus.
 */
class SignalManager::Impl {
public:
    /**
     * @brief Construct the implementation
     * @param parent Pointer to the owning SignalManager instance
     */
    explicit Impl(SignalManager* parent) : parent_(parent) {
        DEBUG_LOG("Initializing SignalManager implementation");
        hook_ = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, GetModuleHandle(nullptr), 0);
        if (hook_ == nullptr) {
            DEBUG_LOG("Failed to set keyboard hook");
        }
    }

    Impl(const Impl&) = delete;
    auto operator=(const Impl&) -> Impl& = delete;
    Impl(Impl&&) = delete;
    auto operator=(Impl&&) -> Impl& = delete;

    /**
     * @brief Destroy the implementation
     *
     * Ensures proper cleanup of the keyboard hook.
     */
    ~Impl() {
        if (hook_ != nullptr) {
            UnhookWindowsHookEx(hook_);
            hook_ = nullptr;
        }
    }

    /**
     * @brief Get the parent SignalManager instance
     * @return Pointer to the parent SignalManager
     */
    [[nodiscard]] auto getParent() const -> SignalManager* { return parent_; }

private:
    /**
     * @brief Windows keyboard hook callback
     *
     * This callback is invoked by Windows when keyboard events occur.
     * It triggers signal checks when relevant keyboard events are detected.
     */
    static auto CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) -> LRESULT {
        if (nCode == HC_ACTION) {
            if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN || wParam == WM_KEYUP || wParam == WM_SYSKEYUP) {
                // Get the Impl instance from the hook handle
                auto* impl = reinterpret_cast<Impl*>(GetProp(GetActiveWindow(), L"SignalManagerImpl"));
                if (impl != nullptr && impl->getParent() != nullptr) {
                    impl->getParent()->checkSignals();
                }
            }
        }
        return CallNextHookEx(nullptr, nCode, wParam, lParam);
    }

    HHOOK hook_{nullptr};    ///< Windows keyboard hook handle
    SignalManager* parent_;  ///< Pointer to the owning SignalManager
};

SignalManager::SignalManager() : pImpl_(std::make_unique<Impl>(this)) {
    DEBUG_LOG("Initializing SignalManager");
    // Store the Impl pointer as a window property for the hook callback
    SetProp(GetActiveWindow(), L"SignalManagerImpl", reinterpret_cast<HANDLE>(pImpl_.get()));
}

SignalManager::~SignalManager() {
    // Remove the window property
    RemoveProp(GetActiveWindow(), L"SignalManagerImpl");
}

auto SignalManager::addSignal(std::unique_ptr<ISignal> signal) -> void {
    DEBUG_LOG("Adding signal to manager");
    signals_.push_back(std::move(signal));
}

auto SignalManager::startSignals() -> void {
    DEBUG_LOG("Starting signals");
    for (const auto& signal : signals_) {
        signal->start();
    }
}

auto SignalManager::stopSignals() -> void {
    DEBUG_LOG("Stopping signals");
    for (const auto& signal : signals_) {
        signal->stop();
    }
}

auto SignalManager::checkSignals() -> void {
    for (const auto& signal : signals_) {
        signal->check();
    }
}

}  // namespace interview_cheater::signal