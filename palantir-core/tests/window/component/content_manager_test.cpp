#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>
#include <nlohmann/json.hpp>

#include "window/component/content_manager_impl.hpp"
#include "mock/window/component/mock_view.hpp"
#include "mock/window/component/mock_content_size_observer.hpp"
#include "mock/window/component/message/mock_message_strategy.hpp"
#include "exception/exceptions.hpp"

using namespace palantir::window::component;
using namespace palantir::test;
using namespace testing;

// necessary for testing coverage, as this class is templated, coverage tool will not count it if not present in one of the modules binary
template class ContentManager<MockView>;

// Test fixture for ContentManagerImpl
class ContentManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        mockView = std::make_shared<NiceMock<MockView>>();
        contentManager = std::make_shared<ContentManager<MockView>>(mockView);
        mockObserver = std::make_unique<StrictMock<MockContentSizeObserver>>();
    }

    void TearDown() override {
        mockObserver.reset();
        contentManager.reset();
        mockView.reset();
    }

    std::shared_ptr<MockView> mockView;
    std::shared_ptr<ContentManager<MockView>> contentManager;
    std::unique_ptr<MockContentSizeObserver> mockObserver;
};

TEST_F(ContentManagerTest, Initialize_CallsViewInitializeAndLoadsURL) {
    uintptr_t mockHandle = 12345;
    
    EXPECT_CALL(*mockView, initialize(mockHandle, ::testing::_))
        .WillOnce(::testing::Invoke([](uintptr_t, std::function<void()> callback) {
            callback(); // Call the initialization callback
        }));
    
    EXPECT_CALL(*mockView, loadURL("http://www.google.com"));
    
    contentManager->initialize(mockHandle);
}

TEST_F(ContentManagerTest, SetRootContent_ValidJson_UpdatesContentAndWebView) {
    std::string validJson = R"({"explanation": "test explanation", "response": "test response"})";
    
    EXPECT_CALL(*mockView, executeJavaScript(::testing::_))
        .Times(1);
    
    contentManager->setRootContent(validJson);
    
    // Verify content was updated by checking getContent
    EXPECT_EQ(contentManager->getContent("explanation"), "test explanation");
    EXPECT_EQ(contentManager->getContent("response"), "test response");
}

TEST_F(ContentManagerTest, SetRootContent_InvalidJson_ThrowsException) {
    std::string invalidJson = "not a valid json";
    
    EXPECT_THROW(contentManager->setRootContent(invalidJson), 
                 palantir::exception::TraceableContentManagerException);
}

TEST_F(ContentManagerTest, SetContent_ValidElementId_UpdatesContentAndWebView) {
    // First set valid root content
    std::string validJson = R"({"explanation": "", "response": ""})";
    contentManager->setRootContent(validJson);
    
    EXPECT_CALL(*mockView, executeJavaScript(::testing::_))
        .Times(1);
    
    contentManager->setContent("explanation", "updated explanation");
    
    EXPECT_EQ(contentManager->getContent("explanation"), "updated explanation");
}

TEST_F(ContentManagerTest, SetContent_ComplexityField_UpdatesContentAndWebView) {
    // First set valid root content
    std::string validJson = R"({
        "explanation": "",
        "response": "",
        "complexity": {
            "time": {"value": "", "explanation": ""},
            "space": {"value": "", "explanation": ""}
        }
    })";
    contentManager->setRootContent(validJson);
    
    EXPECT_CALL(*mockView, executeJavaScript(::testing::_))
        .Times(1);
    
    contentManager->setContent("complexity.time.value", "O(n)");
    
    EXPECT_EQ(contentManager->getContent("complexity.time.value"), "O(n)");
}

TEST_F(ContentManagerTest, GetContent_InvalidElementId_ThrowsException) {
    std::string validJson = R"({"explanation": "", "response": ""})";
    contentManager->setRootContent(validJson);
    
    EXPECT_THROW(contentManager->getContent("invalid_field"), 
                 palantir::exception::TraceableContentManagerException);
}

TEST_F(ContentManagerTest, ToggleContentVisibility_CallsExecuteJavaScript) {
    EXPECT_CALL(*mockView, executeJavaScript(::testing::_))
        .Times(1);
    
    contentManager->toggleContentVisibility("explanation");
}

TEST_F(ContentManagerTest, SetContentVisibility_CallsExecuteJavaScript) {
    EXPECT_CALL(*mockView, executeJavaScript(::testing::_))
        .Times(1);
    
    contentManager->setContentVisibility("explanation", true);
}

TEST_F(ContentManagerTest, GetContentVisibility_ReturnsTrue) {
    // Default implementation returns true
    EXPECT_TRUE(contentManager->getContentVisibility("explanation"));
}

TEST_F(ContentManagerTest, Destroy_CallsViewDestroy) {
    EXPECT_CALL(*mockView, destroy())
        .Times(1);
    
    contentManager->destroy();
}

TEST_F(ContentManagerTest, Resize_UpdatesSizeAndNotifiesObservers) {
    int testWidth = 800;
    int testHeight = 600;
    
    contentManager->addContentSizeObserver(mockObserver.get());
    
    EXPECT_CALL(*mockView, resize(testWidth, testHeight))
        .Times(1);
    
    EXPECT_CALL(*mockObserver, onContentSizeChanged(testWidth, testHeight))
        .Times(1);
    
    contentManager->resize(testWidth, testHeight);
    
    EXPECT_EQ(contentManager->getContentWidth(), testWidth);
    EXPECT_EQ(contentManager->getContentHeight(), testHeight);
}

TEST_F(ContentManagerTest, AddContentSizeObserver_NotifiesObserverOfCurrentSize) {
    int testWidth = 800;
    int testHeight = 600;
    
    // First set a size
    EXPECT_CALL(*mockView, resize(testWidth, testHeight))
        .Times(1);
    contentManager->resize(testWidth, testHeight);
    
    // When adding observer, it should be notified of current size
    EXPECT_CALL(*mockObserver, onContentSizeChanged(testWidth, testHeight))
        .Times(1);
    
    contentManager->addContentSizeObserver(mockObserver.get());
}

TEST_F(ContentManagerTest, AddContentSizeObserver_DuplicateObserver_OnlyAddsOnce) {
    contentManager->addContentSizeObserver(mockObserver.get());
    contentManager->addContentSizeObserver(mockObserver.get()); // Add same observer again
    
    int testWidth = 800;
    int testHeight = 600;
    
    // Observer should only be notified once if it was added multiple times
    EXPECT_CALL(*mockObserver, onContentSizeChanged(testWidth, testHeight))
        .Times(1);
    
    EXPECT_CALL(*mockView, resize(testWidth, testHeight))
        .Times(1);
    
    contentManager->resize(testWidth, testHeight);
}

TEST_F(ContentManagerTest, RemoveContentSizeObserver_RemovedObserverNotNotified) {
    contentManager->addContentSizeObserver(mockObserver.get());
    contentManager->removeContentSizeObserver(mockObserver.get());
    
    int testWidth = 800;
    int testHeight = 600;
    
    // Observer should not be notified after removal
    EXPECT_CALL(*mockObserver, onContentSizeChanged(::testing::_, ::testing::_))
        .Times(0);
    
    EXPECT_CALL(*mockView, resize(testWidth, testHeight))
        .Times(1);
    
    contentManager->resize(testWidth, testHeight);
}

TEST_F(ContentManagerTest, RegisterMessageStrategy_CallsViewRegisterMessageStrategy) {
    auto mockStrategy = std::make_unique<MockMessageStrategy>();
    
    EXPECT_CALL(*mockView, registerMessageStrategy(::testing::_))
        .Times(1);
    
    contentManager->registerMessageStrategy(std::move(mockStrategy));
}

TEST_F(ContentManagerTest, HandleMessage_CallsViewHandleMessage) {
    std::string testMessage = "test message";
    
    EXPECT_CALL(*mockView, handleMessage(testMessage))
        .Times(1);
    
    contentManager->handleMessage(testMessage);
}

TEST_F(ContentManagerTest, SplitMethod_CorrectlySplitsString) {
    std::string validJson = R"({"explanation": "", "response": ""})";
    contentManager->setRootContent(validJson);
    
    // Test a complexity path that requires splitting
    std::string validJson2 = R"({
        "explanation": "",
        "response": "",
        "complexity": {
            "time": {"value": "", "explanation": ""},
            "space": {"value": "", "explanation": ""}
        }
    })";
    contentManager->setRootContent(validJson2);
    contentManager->setContent("complexity.time.value", "O(n)");
    
    // If split works correctly, we should be able to get the value
    EXPECT_EQ(contentManager->getContent("complexity.time.value"), "O(n)");
}
