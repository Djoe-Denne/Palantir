#pragma once

#include "mock/palantir_mock.hpp"
#include "window/iwindow.hpp"

namespace palantir::test {

class MockWindow : public window::IWindow, public PalantirMock {
public:
    ~MockWindow() override = default;
    MOCK_METHOD(void, create, (), (override));
    MOCK_METHOD(void, show, (), (override));
    MOCK_METHOD(void, update, (), (override));
    MOCK_METHOD(void, close, (), (override));
    MOCK_METHOD(void, setTransparency, (int), (override));
    MOCK_METHOD(void, toggleWindowAnonymity, (), (override));
    MOCK_METHOD(bool, isRunning, (), (const, override));
    MOCK_METHOD(void, setRunning, (bool), (override));
    MOCK_METHOD(void*, getNativeHandle, (), (const, override));
    MOCK_METHOD(std::shared_ptr<window::component::IContentManager>, getContentManager, (), (const, override));
    MOCK_METHOD(const window::WindowType&, getWindowType, (), (const, override));
};

} // namespace palantir::test 