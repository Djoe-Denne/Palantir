#pragma once

#include "mock/palantir_mock.hpp"
#include "window/component/message/message_handler.hpp"

namespace palantir::test {

class MockView : public PalantirMock {
public:
    MockView() = default;
    ~MockView() = default;

    MOCK_METHOD(void, initialize, (uintptr_t nativeWindowHandle, std::function<void()> onInitialized));
    MOCK_METHOD(void, loadURL, (const std::string& url));
    MOCK_METHOD(void, executeJavaScript, (const std::string& script));
    MOCK_METHOD(void, destroy, ());
    MOCK_METHOD(void, resize, (int width, int height));
    MOCK_METHOD(void, registerMessageStrategy, (std::unique_ptr<window::component::message::MessageStrategyBase> strategy));
    MOCK_METHOD(void, handleMessage, (const std::string& message));
};

} // namespace palantir::test 