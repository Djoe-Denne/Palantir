#pragma once

#include <gmock/gmock.h>
#include "window/window_manager.hpp"
#include "mocks/mock_window.hpp"

namespace palantir::test {

class MockWindowManager : public window::WindowManager {
public:
    MockWindowManager() : WindowManager() {}

    MOCK_METHOD(std::shared_ptr<window::IWindow>, getFirstWindow, (), (const, override));
    MOCK_METHOD(void, addWindow, (const std::shared_ptr<window::IWindow>&), (override));
    MOCK_METHOD(void, removeWindow, (const window::IWindow*), (override));
    MOCK_METHOD(bool, hasRunningWindows, (), (const, override));
};

} // namespace palantir::test 