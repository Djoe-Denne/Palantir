#include "signal/signal_manager.hpp"

#include <windows.h>
#include <vector>

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
     */
    ~Impl() {
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
    auto startSignals() -> void {
        for (const auto& signal : signals_) {
            signal->start();
        }
    }

    /**
     * @brief Stop all signals
     */
    auto stopSignals() -> void {
        for (const auto& signal : signals_) {
            signal->stop();
        }
    }

    /**
     * @brief Check all signals
     */
    auto checkSignals(const std::any& event) -> void {
        for (const auto& signal : signals_) {
            signal->check(event);
        }
    }

private:
    /**
     * @brief Windows keyboard hook callback
     */
    static auto CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) -> LRESULT {
        if (nCode == HC_ACTION) {
            if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN || wParam == WM_KEYUP || wParam == WM_SYSKEYUP) {
                // Get the Impl instance from the hook handle
                auto* impl = reinterpret_cast<Impl*>(GetProp(GetActiveWindow(), L"SignalManagerImpl"));
                if (impl != nullptr) {
                    impl->checkSignals(nullptr);
                }
            }
        }
        return CallNextHookEx(nullptr, nCode, wParam, lParam);
    }

    HHOOK hook_{nullptr};                                  ///< Windows keyboard hook handle
    SignalManager* parent_;                                ///< Pointer to the owning SignalManager
    std::vector<std::unique_ptr<ISignal>> signals_;       ///< Collection of managed signals
};

// Singleton instance
auto SignalManager::getInstance() -> SignalManager& {
    static SignalManager instance;
    return instance;
}

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
    pImpl_->addSignal(std::move(signal));
}

auto SignalManager::startSignals() -> void {
    DEBUG_LOG("Starting signals");
    pImpl_->startSignals();
}

auto SignalManager::stopSignals() -> void {
    DEBUG_LOG("Stopping signals");
    pImpl_->stopSignals();
}

auto SignalManager::checkSignals(const std::any& event) -> void {
    pImpl_->checkSignals(event);
}

}  // namespace interview_cheater::signal