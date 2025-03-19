#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "client/sauron_register.hpp"
#include "mock/client/mock_sauron_client.hpp"

using namespace palantir::client;
using namespace palantir::test;
using ::testing::Return;
using ::testing::_;

class SauronRegisterTestable : public SauronRegister {
public:
    SauronRegisterTestable() = default;
    SauronRegisterTestable(const std::shared_ptr<MockSauronClient>& sauronClient) : SauronRegister(sauronClient) {}
};

class SauronRegisterTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Reset the singleton instance before each test
        SauronRegister::setInstance(nullptr);
    }
    
    void TearDown() override {
        // Reset the singleton instance after each test
        SauronRegister::setInstance(nullptr);
    }
};

TEST_F(SauronRegisterTest, GetInstanceReturnsNonNullInstance) {
    // Act
    auto instance = SauronRegister::getInstance();
    
    // Assert
    ASSERT_NE(instance, nullptr);
}

TEST_F(SauronRegisterTest, GetInstanceReturnsSameInstanceOnMultipleCalls) {
    // Act
    auto instance1 = SauronRegister::getInstance();
    auto instance2 = SauronRegister::getInstance();
    
    // Assert
    EXPECT_EQ(instance1, instance2);
}

TEST_F(SauronRegisterTest, SetInstanceOverridesDefaultInstance) {
    // Arrange
    auto mockClient = std::make_shared<MockSauronClient>();
    
    auto customInstance = std::make_shared<SauronRegisterTestable>(mockClient);
    
    // Act
    SauronRegister::setInstance(customInstance);
    auto retrievedInstance = SauronRegister::getInstance();
    
    // Assert
    EXPECT_EQ(retrievedInstance, customInstance);
}

TEST_F(SauronRegisterTest, SetInstanceWithNullptrResetsInstance) {
    // Arrange
    auto initialInstance = SauronRegister::getInstance();
    
    // Act
    SauronRegister::setInstance(nullptr);
    auto newInstance = SauronRegister::getInstance();
    
    // Assert
    EXPECT_NE(initialInstance, newInstance);
}

TEST_F(SauronRegisterTest, GetSauronClientReturnsNonNullClient) {
    // Arrange
    auto instance = SauronRegister::getInstance();
    
    // Act
    auto client = instance->getSauronClient();
    
    // Assert
    EXPECT_NE(client, nullptr);
}

TEST_F(SauronRegisterTest, CustomClientIsAccessibleViaGetSauronClient) {
    // Arrange
    auto mockClient = std::make_shared<MockSauronClient>();
    auto customInstance = std::make_shared<SauronRegisterTestable>(mockClient);
    SauronRegister::setInstance(customInstance);
    
    // Act
    auto retrievedClient = SauronRegister::getInstance()->getSauronClient();
    
    // Assert
    EXPECT_EQ(retrievedClient, mockClient);
}

TEST_F(SauronRegisterTest, ConstructorWithClientParameter) {
    // Arrange
    auto mockClient = std::make_shared<MockSauronClient>();
    
    // Act
    SauronRegisterTestable register_(mockClient);
    auto retrievedClient = register_.getSauronClient();
    
    // Assert
    EXPECT_EQ(retrievedClient, mockClient);
}

TEST_F(SauronRegisterTest, DefaultConstructorCreatesClient) {
    // Arrange & Act
    SauronRegisterTestable register_;
    auto client = register_.getSauronClient();
    
    // Assert
    EXPECT_NE(client, nullptr);
}

TEST_F(SauronRegisterTest, DestructorDoesNotAffectStaticInstance) {
    // Arrange
    std::shared_ptr<SauronRegister> instancePtr;
    {
        auto mockClient = std::make_shared<MockSauronClient>();
        instancePtr = std::make_shared<SauronRegisterTestable>(mockClient);
        SauronRegister::setInstance(instancePtr);
    }
    
    // Act & Assert - The reference count should prevent destruction
    EXPECT_EQ(SauronRegister::getInstance(), instancePtr);
}

// Integration test with mock
TEST_F(SauronRegisterTest, ClientMethodsCanBeCalledThroughRegister) {
    // Arrange
    auto mockClient = std::make_shared<MockSauronClient>();
    auto loginResponse = sauron::dto::TokenResponse("token123");
    
    EXPECT_CALL(*mockClient, login(_))
        .WillOnce(Return(loginResponse));
    
    auto customInstance = std::make_shared<SauronRegisterTestable>(mockClient);
    SauronRegister::setInstance(customInstance);
    
    // Act
    auto client = SauronRegister::getInstance()->getSauronClient();
    auto response = client->login(sauron::dto::LoginRequest("api-key", sauron::dto::AIProvider::OPENAI));
    
    // Assert
    EXPECT_EQ(response.getToken(), "token123");
} 