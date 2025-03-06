#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "input/key_register.hpp"

using namespace palantir::input;
using namespace testing;

class KeyRegisterTest : public Test {
protected:
    void SetUp() override {
        // Get the singleton instance
        keyRegister = KeyRegister::getInstance();
    }

    void TearDown() override {
        // No teardown needed for singleton
    }

    std::shared_ptr<KeyRegister> keyRegister;
};

TEST_F(KeyRegisterTest, RegisterKey_NewKey_CanBeRetrieved) {
    // Register a new key
    const std::string testKey = "TestKey";
    const int testValue = 42;
    
    keyRegister->registerKey(testKey, testValue);
    
    // Verify the key was registered
    EXPECT_TRUE(keyRegister->hasKey(testKey));
    EXPECT_EQ(keyRegister->get(testKey), testValue);
}

TEST_F(KeyRegisterTest, RegisterKey_ExistingKey_UpdatesValue) {
    // Register a key
    const std::string testKey = "TestKey2";
    const int initialValue = 100;
    const int updatedValue = 200;
    
    keyRegister->registerKey(testKey, initialValue);
    EXPECT_EQ(keyRegister->get(testKey), initialValue);
    
    // Update the key with a new value
    keyRegister->registerKey(testKey, updatedValue);
    EXPECT_EQ(keyRegister->get(testKey), updatedValue);
}

TEST_F(KeyRegisterTest, HasKey_NonExistentKey_ReturnsFalse) {
    // Test with a key that hasn't been registered
    const std::string nonExistentKey = "NonExistentKey";
    
    EXPECT_FALSE(keyRegister->hasKey(nonExistentKey));
}

TEST_F(KeyRegisterTest, Get_NonExistentKey_ThrowsException) {
    // Test getting a key that hasn't been registered
    const std::string nonExistentKey = "NonExistentKey";
    
    EXPECT_THROW(keyRegister->get(nonExistentKey), std::invalid_argument);
}

TEST_F(KeyRegisterTest, Singleton_GetInstance_ReturnsSameInstance) {
    // Test that getInstance returns the same instance
    auto instance1 = KeyRegister::getInstance();
    auto instance2 = KeyRegister::getInstance();
    
    EXPECT_EQ(instance1, instance2);
} 