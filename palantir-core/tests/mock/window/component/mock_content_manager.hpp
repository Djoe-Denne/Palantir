#pragma once

#include "mock/palantir_mock.hpp"
#include "window/component/icontent_manager.hpp"

namespace palantir::test {

class MockContentManager : public window::component::IContentManager, public PalantirMock {
public:
    MockContentManager() = default;
    ~MockContentManager() override = default;

    MOCK_METHOD(void, initialize, (void* nativeWindowHandle), (override));
    MOCK_METHOD(void, setRootContent, (const std::string& content), (override));
    MOCK_METHOD(void, setContent, (const std::string& elementId, const std::string& content), (override));
    MOCK_METHOD(std::string, getContent, (const std::string& elementId), (override));
    MOCK_METHOD(void, toggleContentVisibility, (const std::string& elementId), (override));
    MOCK_METHOD(void, setContentVisibility, (const std::string& elementId, bool visible), (override));
    MOCK_METHOD(bool, getContentVisibility, (const std::string& elementId), (override));
    MOCK_METHOD(void, destroy, (), (override));
    MOCK_METHOD(void, resize, (int width, int height), (override));
};

} // namespace palantir::test 