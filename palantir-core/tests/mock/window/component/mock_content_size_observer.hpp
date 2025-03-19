#pragma once

#include "mock/palantir_mock.hpp"
#include "window/component/icontent_size_observer.hpp"

namespace palantir::test {

class MockContentSizeObserver : public window::component::IContentSizeObserver, public PalantirMock {
public:
    MockContentSizeObserver() = default;
    ~MockContentSizeObserver() override = default;

    MOCK_METHOD(void, onContentSizeChanged, (int width, int height), (override));
};

} // namespace palantir::test 