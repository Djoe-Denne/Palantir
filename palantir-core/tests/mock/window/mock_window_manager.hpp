#pragma once

#include "mock/palantir_mock.hpp"
#include "window/window_manager.hpp"

namespace palantir::test {

class MockWindowManager : public window::WindowManager, public PalantirMock {
public:
    MockWindowManager() : WindowManager() {}
    ~MockWindowManager() override = default;

    MOCK_METHOD(std::shared_ptr<window::IWindow>, getMainWindow, (), (const, override));
    MOCK_METHOD(void, addWindow, (const std::shared_ptr<window::IWindow>&), (override));
    MOCK_METHOD(void, removeWindow, (const window::IWindow*), (override));
    MOCK_METHOD(bool, hasRunningWindows, (), (const, override));
};

} // namespace palantir::test 