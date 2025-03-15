#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>

#include "window/component/message/resize/resize_strategy.hpp"
#include "window/component/message/resize/resize_message_vo.hpp"
#include "mock/window/component/mock_content_manager.hpp"

using namespace palantir::window::component::message::resize;
using namespace palantir::test;
using namespace testing;

class ResizeStrategyTest : public Test {
protected:
    void SetUp() override {
        mockContentManager = std::make_shared<MockContentManager>();
        strategy = std::make_unique<ResizeStrategy>("resize", mockContentManager);
    }

    void TearDown() override {
        mockContentManager.reset();
        strategy.reset();
    }

    std::shared_ptr<MockContentManager> mockContentManager;
    std::unique_ptr<ResizeStrategy> strategy;
};

TEST_F(ResizeStrategyTest, Constructor_SetsEventType) {
    EXPECT_EQ(strategy->getEventType(), "resize");
}

TEST_F(ResizeStrategyTest, Execute_CallsContentManagerResize) {
    ResizeMessageVO message{800, 600};
    
    EXPECT_CALL(*mockContentManager, resize(800, 600))
        .Times(1);
    
    strategy->execute(message);
}

TEST_F(ResizeStrategyTest, Execute_ZeroValues_CallsContentManagerResize) {
    ResizeMessageVO message{0, 0};
    
    EXPECT_CALL(*mockContentManager, resize(0, 0))
        .Times(1);
    
    strategy->execute(message);
}

TEST_F(ResizeStrategyTest, Execute_NegativeValues_CallsContentManagerResize) {
    ResizeMessageVO message{-100, -200};
    
    EXPECT_CALL(*mockContentManager, resize(-100, -200))
        .Times(1);
    
    strategy->execute(message);
} 