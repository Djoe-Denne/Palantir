#pragma once

#include <gmock/gmock.h>
#include "window/iwindow.hpp"

namespace interview_cheater::test {

class MockWindow : public window::IWindow {
public:
    ~MockWindow() override = default;
    MOCK_METHOD(void, create, (), (override));
    MOCK_METHOD(void, show, (), (override));
    MOCK_METHOD(void, update, (), (override));
    MOCK_METHOD(void, close, (), (override));
    MOCK_METHOD(bool, isRunning, (), (const, override));
    MOCK_METHOD(void, setRunning, (bool), (override));
    MOCK_METHOD(void*, getNativeHandle, (), (const, override));
};

} // namespace interview_cheater::test 