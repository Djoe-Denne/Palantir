#pragma once

#include "signal/keyboard_api.hpp"
#include "mock/palantir_mock.hpp"
#include <gmock/gmock.h>

namespace palantir::test {

/**
 * @brief Mock implementation of keyboard API for testing
 * 
 * This class provides a mock implementation of the keyboard API
 * wrapper for testing purposes. It uses Google Mock to allow setting
 * expectations and verifying calls to the API methods.
 */
class KeyboardApiMock : public palantir::signal::KeyboardApi, public palantir::test::PalantirMock {
public:
    MOCK_CONST_METHOD4(SetHook, HHOOK(int idHook, HOOKPROC lpfn, HINSTANCE hMod, DWORD dwThreadId));
    MOCK_CONST_METHOD1(UnhookKeyboard, BOOL(HHOOK hhk));
    MOCK_CONST_METHOD4(CallNextHook, LRESULT(HHOOK hhk, int nCode, WPARAM wParam, LPARAM lParam));
    MOCK_CONST_METHOD1(GetModuleOSHandle, HINSTANCE(LPCWSTR lpModuleName));
};
} // namespace palantir::test::mock 