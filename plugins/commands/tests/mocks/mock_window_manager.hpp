#pragma once

#include <gmock/gmock.h>
#include "window/window_manager.hpp"
#include "mocks/mock_window.hpp"

namespace interview_cheater::test {

class MockWindowManager : public window::WindowManager {
public:
    MockWindowManager() : WindowManager() {}

    MOCK_METHOD(std::shared_ptr<window::IWindow>, getFirstWindow, (), (const, override));
    MOCK_METHOD(void, addWindow, (std::unique_ptr<window::IWindow>), (override));
    MOCK_METHOD(void, removeWindow, (const window::IWindow*), (override));
    MOCK_METHOD(bool, hasRunningWindows, (), (const, override));
};

} // namespace interview_cheater::test 