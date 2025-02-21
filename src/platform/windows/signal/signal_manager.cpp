#include "signal/signal_manager.hpp"

#include <windows.h>

#include "signal/isignal.hpp"
#include "utils/logger.hpp"

namespace {
HHOOK g_keyboardHook = nullptr;
interview_cheater::signal::SignalManager* g_signalManager = nullptr;

auto CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) -> LRESULT {
    if (nCode == HC_ACTION) {
        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN || wParam == WM_KEYUP || wParam == WM_SYSKEYUP) {
            if (g_signalManager != nullptr) {
                g_signalManager->checkSignals();
            }
        }
    }
    return CallNextHookEx(nullptr, nCode, wParam, lParam);
}

}  // namespace

namespace interview_cheater::signal {

SignalManager::SignalManager() {
    g_signalManager = this;
    g_keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, GetModuleHandle(nullptr), 0);
    if (g_keyboardHook == nullptr) {
        DEBUG_LOG("Failed to set keyboard hook");
    }
}

SignalManager::~SignalManager() {
    if (g_keyboardHook != nullptr) {
        UnhookWindowsHookEx(g_keyboardHook);
        g_keyboardHook = nullptr;
    }
    g_signalManager = nullptr;
}

auto SignalManager::addSignal(std::unique_ptr<ISignal> signal) -> void { signals_.push_back(std::move(signal)); }

auto SignalManager::startSignals() -> void {
    for (const auto& signal : signals_) {
        signal->start();
    }
}

auto SignalManager::stopSignals() -> void {
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