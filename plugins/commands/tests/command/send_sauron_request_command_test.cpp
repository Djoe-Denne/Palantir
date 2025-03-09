#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <filesystem>
#include <fstream>

#include "command/send_sauron_request_command.hpp"
#include "mock/mock_application.hpp"
#include "mock/window/mock_window_manager.hpp"
#include "mock/window/mock_window.hpp"
#include "mock/window/component/mock_content_manager.hpp"
#include "mock/client/mock_sauron_client.hpp"
#include "mock/client/mock_sauron_register.hpp"
#include "sauron/dto/DTOs.hpp"

using namespace palantir;
using namespace palantir::command;
using namespace palantir::test;
using namespace testing;

class SendSauronRequestCommandTest : public Test {
protected:
    void SetUp() override {
        mockWindow = std::make_shared<MockWindow>();
        mockWindowManager = std::make_shared<MockWindowManager>();
        mockApp = std::make_shared<MockApplication>("");
        mockContentManager = std::make_shared<MockContentManager>();
        mockSauronClient = std::make_shared<MockSauronClient>();
        mockSauronRegister = std::make_shared<MockSauronRegister>(mockSauronClient);

        // Set the mock client in the register
        palantir::client::SauronRegister::setInstance(mockSauronRegister);
        palantir::Application::setInstance(mockApp);
        palantir::window::WindowManager::setInstance(mockWindowManager);

        // If ./screenshot directory does not exist, create it
        if (!std::filesystem::exists("./screenshot")) {
            std::filesystem::create_directory("./screenshot");
        }
        // clear the screenshot directory
        for (const auto& file : std::filesystem::directory_iterator("./screenshot")) {
            std::filesystem::remove(file);
        }
    }

    void TearDown() override {
        palantir::client::SauronRegister::setInstance(nullptr);
        palantir::Application::setInstance(nullptr);
        palantir::window::WindowManager::setInstance(nullptr);
        
        mockApp.reset();
        mockWindow.reset();
        mockWindowManager.reset();
        mockContentManager.reset();
        mockSauronClient.reset();
        mockSauronRegister.reset();

        // clear the screenshot directory
        for (const auto& file : std::filesystem::directory_iterator("./screenshot")) {
            std::filesystem::remove(file);
        }
    }

    std::shared_ptr<MockWindow> mockWindow;
    std::shared_ptr<MockWindowManager> mockWindowManager;
    std::shared_ptr<MockApplication> mockApp;
    std::shared_ptr<MockContentManager> mockContentManager;
    std::shared_ptr<MockSauronClient> mockSauronClient;
    std::shared_ptr<MockSauronRegister> mockSauronRegister;
};

namespace success {
TEST_F(SendSauronRequestCommandTest, ExecuteSetsContentWhenWindowExists) {
    // Arrange
    const std::string testPrompt = "Test prompt";
    const std::string expectedResponse = R"({"response": "Test response"})";
    
    EXPECT_CALL(*mockWindowManager, getMainWindow())
        .WillOnce(Return(mockWindow));
        
    EXPECT_CALL(*mockApp, getWindowManager())
        .WillRepeatedly(Return(mockWindowManager));
    
    EXPECT_CALL(*mockWindow, getContentManager())
        .WillRepeatedly(Return(mockContentManager));
    
    EXPECT_CALL(*mockSauronRegister, getSauronClient())
        .WillOnce(Return(mockSauronClient));
    
    sauron::dto::AIAlgorithmResponse mockResponse;
    mockResponse.setResponse("Test response");
    
    EXPECT_CALL(*mockSauronClient, queryAlgorithm(_))
        .WillOnce(Return(mockResponse));
    
    EXPECT_CALL(*mockContentManager, setRootContent(HasSubstr("Test response")))
        .Times(1);
    
    SendSauronRequestCommand command(testPrompt);

    // Act
    command.execute();
}

TEST_F(SendSauronRequestCommandTest, ExecuteSendPrompt) {
    // Arrange
    const std::string testPrompt = "Test prompt";

    EXPECT_CALL(*mockWindowManager, getMainWindow())
        .WillOnce(Return(mockWindow));
        
    EXPECT_CALL(*mockApp, getWindowManager())
        .WillRepeatedly(Return(mockWindowManager));
    
    EXPECT_CALL(*mockWindow, getContentManager())
        .WillRepeatedly(Return(mockContentManager));
    
    EXPECT_CALL(*mockSauronRegister, getSauronClient())
        .WillOnce(Return(mockSauronClient));
    
    sauron::dto::AIAlgorithmResponse mockResponse;
    mockResponse.setResponse("Test response");
    
    
    EXPECT_CALL(*mockSauronClient, queryAlgorithm(
        Property(&sauron::dto::AIQueryRequest::getPrompt, Eq(testPrompt))))
        .WillOnce(Return(mockResponse));
        
    EXPECT_CALL(*mockContentManager, setRootContent(HasSubstr("Test response")))
        .Times(1);
    
    SendSauronRequestCommand command(testPrompt);

    // Act
    command.execute();
}

TEST_F(SendSauronRequestCommandTest, ExecuteReadScreenshotImage) {
    // Arrange
    const std::string testPrompt = "Test prompt";
    
    // create a screenshot file
    std::ofstream screenshotFile("./screenshot/screenshot.png");
    screenshotFile << "Test screenshot";
    screenshotFile.close();


    EXPECT_CALL(*mockWindowManager, getMainWindow())
        .WillOnce(Return(mockWindow));
        
    EXPECT_CALL(*mockApp, getWindowManager())
        .WillRepeatedly(Return(mockWindowManager));
    
    EXPECT_CALL(*mockWindow, getContentManager())
        .WillRepeatedly(Return(mockContentManager));
    
    EXPECT_CALL(*mockSauronRegister, getSauronClient())
        .WillOnce(Return(mockSauronClient));
    
    sauron::dto::AIAlgorithmResponse mockResponse;
    mockResponse.setResponse("Test response");
    
    
    EXPECT_CALL(*mockSauronClient, queryAlgorithm(
        Property(&sauron::dto::AIQueryRequest::getImages, Not(IsEmpty()))))
        .WillOnce(Return(mockResponse));
        
    EXPECT_CALL(*mockContentManager, setRootContent(HasSubstr("Test response")))
        .Times(1);
    
    SendSauronRequestCommand command(testPrompt);

    // Act
    command.execute();
}

TEST_F(SendSauronRequestCommandTest, ExecuteSendAIProvider) {
    // Arrange
    const std::string testPrompt = "Test prompt";
 
    EXPECT_CALL(*mockWindowManager, getMainWindow())
        .WillOnce(Return(mockWindow));
        
    EXPECT_CALL(*mockApp, getWindowManager())
        .WillRepeatedly(Return(mockWindowManager));
    
    EXPECT_CALL(*mockWindow, getContentManager())
        .WillRepeatedly(Return(mockContentManager));
    
    EXPECT_CALL(*mockSauronRegister, getSauronClient())
        .WillOnce(Return(mockSauronClient));
    
    sauron::dto::AIAlgorithmResponse mockResponse;
    mockResponse.setResponse("Test response");
    
    
    EXPECT_CALL(*mockSauronClient, queryAlgorithm(
        Property(&sauron::dto::AIQueryRequest::getProvider, Eq(sauron::dto::AIProvider::OPENAI))))
        .WillOnce(Return(mockResponse));
        
    EXPECT_CALL(*mockContentManager, setRootContent(HasSubstr("Test response")))
        .Times(1);
    
    SendSauronRequestCommand command(testPrompt);

    // Act
    command.execute();
}

TEST_F(SendSauronRequestCommandTest, ExecuteSendAIModel) {
    // Arrange
    const std::string testPrompt = "Test prompt";
 
    EXPECT_CALL(*mockWindowManager, getMainWindow())
        .WillOnce(Return(mockWindow));
        
    EXPECT_CALL(*mockApp, getWindowManager())
        .WillRepeatedly(Return(mockWindowManager));
    
    EXPECT_CALL(*mockWindow, getContentManager())
        .WillRepeatedly(Return(mockContentManager));
    
    EXPECT_CALL(*mockSauronRegister, getSauronClient())
        .WillOnce(Return(mockSauronClient));
    
    sauron::dto::AIAlgorithmResponse mockResponse;
    mockResponse.setResponse("Test response");
    
    
    EXPECT_CALL(*mockSauronClient, queryAlgorithm(
        Property(&sauron::dto::AIQueryRequest::getModel, Eq("gpt-4o"))))
        .WillOnce(Return(mockResponse));
        
    EXPECT_CALL(*mockContentManager, setRootContent(HasSubstr("Test response")))
        .Times(1);
    
    SendSauronRequestCommand command(testPrompt);

    // Act
    command.execute();
}

TEST_F(SendSauronRequestCommandTest, UseDebounceReturnsFalse) {
    // Arrange
    const std::string testPrompt = "Test prompt";
    SendSauronRequestCommand command(testPrompt);

    // Act & Assert
    EXPECT_FALSE(command.useDebounce());
} 
} // namespace success

namespace failure {
TEST_F(SendSauronRequestCommandTest, ExecuteThrowsWhenNoWindow) {
    // Arrange
    const std::string testPrompt = "Test prompt";
    
    EXPECT_CALL(*mockWindowManager, getMainWindow())
        .WillOnce(Return(nullptr));
    
    sauron::dto::AIAlgorithmResponse mockResponse;
    mockResponse.setResponse("Test response");

    EXPECT_CALL(*mockSauronRegister, getSauronClient())
        .WillOnce(Return(mockSauronClient));
    
    EXPECT_CALL(*mockSauronClient, queryAlgorithm(_))
        .WillOnce(Return(mockResponse));

    SendSauronRequestCommand command(testPrompt);

    // Act & Assert
    EXPECT_THROW(command.execute(), std::runtime_error);
}

TEST_F(SendSauronRequestCommandTest, ExecuteThrowsWhenNoContentManager) {
    // Arrange
    const std::string testPrompt = "Test prompt";
    
    EXPECT_CALL(*mockWindowManager, getMainWindow())
        .WillOnce(Return(mockWindow));
    
    EXPECT_CALL(*mockWindow, getContentManager())
        .WillOnce(Return(nullptr));

    EXPECT_CALL(*mockSauronRegister, getSauronClient())
        .WillOnce(Return(mockSauronClient));

    sauron::dto::AIAlgorithmResponse mockResponse;
    mockResponse.setResponse("Test response");
    
    EXPECT_CALL(*mockSauronClient, queryAlgorithm(_))
        .WillOnce(Return(mockResponse));
    
    
    SendSauronRequestCommand command(testPrompt);

    // Act & Assert
    EXPECT_THROW(command.execute(), std::runtime_error);
}

TEST_F(SendSauronRequestCommandTest, ExecuteThrowsWhenClientThrows) {
    // Arrange
    const std::string testPrompt = "Test prompt";

    EXPECT_CALL(*mockSauronRegister, getSauronClient())
        .WillOnce(Return(mockSauronClient));
    
    EXPECT_CALL(*mockSauronClient, queryAlgorithm(_))
        .WillOnce(Throw(std::runtime_error("Test error")));
    
    
    SendSauronRequestCommand command(testPrompt);

    // Act & Assert
    EXPECT_THROW(command.execute(), std::runtime_error);
}
} // namespace failure