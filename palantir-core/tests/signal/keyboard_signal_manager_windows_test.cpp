#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>
#include <any>
#include <Windows.h>
#include <detours/detours.h>

#include "signal/keyboard_signal_manager.hpp"
#include "signal/isignal.hpp"
#include "mock/signal/mock_signal.hpp"

using namespace palantir::signal;
using namespace palantir::test;
using namespace testing;

// Mock for Windows API functions
namespace {
    // Windows API function pointers we need to mock
    HHOOK (WINAPI *OriginalSetWindowsHookEx)(int idHook, HOOKPROC lpfn, HINSTANCE hmod, DWORD dwThreadId) = SetWindowsHookEx;
    BOOL (WINAPI *OriginalUnhookWindowsHookEx)(HHOOK hhk) = UnhookWindowsHookEx;
    LRESULT (WINAPI *OriginalCallNextHookEx)(HHOOK hhk, int nCode, WPARAM wParam, LPARAM lParam) = CallNextHookEx;

    // Variables to track mock behavior
    bool setHookFails = false;
    HHOOK mockHookHandle = reinterpret_cast<HHOOK>(1); // Non-null mock handle
    
    // Mock implementations
    HHOOK WINAPI MockSetWindowsHookEx(int idHook, HOOKPROC lpfn, HINSTANCE hmod, DWORD dwThreadId) {
        if (setHookFails) {
            return nullptr;
        }
        return mockHookHandle;
    }
    
    BOOL WINAPI MockUnhookWindowsHookEx(HHOOK hhk) {
        return TRUE;
    }
    
    LRESULT WINAPI MockCallNextHookEx(HHOOK hhk, int nCode, WPARAM wParam, LPARAM lParam) {
        return 0;
    }
    
    // Keyboard hook callback function type
    using KeyboardProcFn = LRESULT (CALLBACK *)(int, WPARAM, LPARAM);
    
    // Capture the keyboard proc for testing
    HOOKPROC capturedKeyboardProc = nullptr;
}

class SignalManagerWindowsTest : public Test {
protected:
    void SetUp() override {
        // Reset global variables
        setHookFails = false;
        capturedKeyboardProc = nullptr;
        
        // Install mocks
        DetourAttach(&(PVOID&)OriginalSetWindowsHookEx, MockSetWindowsHookEx);
        DetourAttach(&(PVOID&)OriginalUnhookWindowsHookEx, MockUnhookWindowsHookEx);
        DetourAttach(&(PVOID&)OriginalCallNextHookEx, MockCallNextHookEx);
        
    }

    void TearDown() override {
        // Remove mocks
        DetourDetach(&(PVOID&)OriginalSetWindowsHookEx, MockSetWindowsHookEx);
        DetourDetach(&(PVOID&)OriginalUnhookWindowsHookEx, MockUnhookWindowsHookEx);
        DetourDetach(&(PVOID&)OriginalCallNextHookEx, MockCallNextHookEx);
    }
};

// Note: The tests below require the Microsoft Detours library for mocking Windows API functions
// and may need to be skipped in environments where that's not available

TEST_F(SignalManagerWindowsTest, KeyboardHook_IsInstalled) {
    // This test verifies that the keyboard hook is installed when SignalManager is created
    auto manager = std::make_shared<KeyboardSignalManager>();
    
    // The hook proc should have been captured
    EXPECT_NE(capturedKeyboardProc, nullptr);
}

TEST_F(SignalManagerWindowsTest, KeyboardHook_CallsCheckSignals) {
    auto manager = std::make_shared<KeyboardSignalManager>();
    auto mockSignal = std::make_unique<MockSignal>();
    auto* mockSignalPtr = mockSignal.get();
    
    // Expect the signal to be checked when key events are received
    EXPECT_CALL(*mockSignalPtr, check(_)).Times(4); // 4 keyboard events
    
    manager->addSignal(std::move(mockSignal));
    
    // Simulate key events
    // Hook code must be HC_ACTION and various key events should trigger checks
    if (capturedKeyboardProc) {
        capturedKeyboardProc(HC_ACTION, WM_KEYDOWN, 0);
        capturedKeyboardProc(HC_ACTION, WM_KEYUP, 0);
        capturedKeyboardProc(HC_ACTION, WM_SYSKEYDOWN, 0);
        capturedKeyboardProc(HC_ACTION, WM_SYSKEYUP, 0);
    }
}

TEST_F(SignalManagerWindowsTest, KeyboardHook_DoesNotCallCheckSignals_WhenNonKeyEvent) {
    auto manager = std::make_shared<KeyboardSignalManager>();
    auto mockSignal = std::make_unique<MockSignal>();
    auto* mockSignalPtr = mockSignal.get();
    
    // Expect the signal not to be checked for non-key events
    EXPECT_CALL(*mockSignalPtr, check(_)).Times(0);
    
    manager->addSignal(std::move(mockSignal));
    
    // Simulate non-key events
    if (capturedKeyboardProc) {
        capturedKeyboardProc(HC_ACTION, WM_MOUSEMOVE, 0);
        capturedKeyboardProc(HC_NOREMOVE, WM_KEYDOWN, 0); // Wrong code
    }
}

TEST_F(SignalManagerWindowsTest, FailedHookInstallation_StillWorks) {
    // Set the hook to fail
    setHookFails = true;
    
    // The SignalManager should still initialize without crashing
    auto manager = std::make_shared<KeyboardSignalManager>();
    auto mockSignal = std::make_unique<MockSignal>();
    auto* mockSignalPtr = mockSignal.get();
    
    EXPECT_CALL(*mockSignalPtr, start()).Times(1);
    EXPECT_CALL(*mockSignalPtr, stop()).Times(1);
    
    manager->addSignal(std::move(mockSignal));
    manager->startSignals();
    manager->stopSignals();
    
    // No assertions here, we're just testing that we don't crash
}