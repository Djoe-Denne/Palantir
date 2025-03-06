#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "input/key_mapper.hpp"
#include "mock/input/mock_key_register.hpp"

using namespace palantir::input;
using namespace palantir::test;
using namespace testing;

class KeyMapperTest : public Test {
protected:
    void SetUp() override {
        mockKeyRegister = std::make_shared<MockKeyRegister>();
        KeyRegister::setInstance(mockKeyRegister);
    }

    void TearDown() override {
        KeyRegister::setInstance(nullptr);
        mockKeyRegister.reset();
    }

    std::shared_ptr<MockKeyRegister> mockKeyRegister;
};

TEST_F(KeyMapperTest, IsValidKey_ValidKeys_ReturnsTrue) {
    // Test a few common keys that should be valid
    EXPECT_CALL(*mockKeyRegister, hasKey("A")).WillOnce(Return(true));
    EXPECT_TRUE(KeyMapper::isValidKey("A"));
}

TEST_F(KeyMapperTest, IsValidKey_InvalidKeys_ReturnsFalse) {
    // Test some invalid keys
    EXPECT_CALL(*mockKeyRegister, hasKey("")).WillOnce(Return(false));
    EXPECT_FALSE(KeyMapper::isValidKey(""));
}

TEST_F(KeyMapperTest, IsValidModifier_ValidModifiers_ReturnsTrue) {
    // Test common modifiers that should be valid
    EXPECT_CALL(*mockKeyRegister, hasKey("CTRL")).WillOnce(Return(true));
    EXPECT_TRUE(KeyMapper::isValidModifier("Ctrl"));
}

TEST_F(KeyMapperTest, IsValidModifier_InvalidModifiers_ReturnsFalse) {
    // Test some invalid modifiers
    EXPECT_CALL(*mockKeyRegister, hasKey("")).WillOnce(Return(false));
    EXPECT_FALSE(KeyMapper::isValidModifier(""));
}

TEST_F(KeyMapperTest, GetKeyCode_ValidKey_ReturnsNonZeroCode) {
    // Test that valid keys return a non-zero key code
    EXPECT_CALL(*mockKeyRegister, hasKey("A")).WillOnce(Return(true));
    EXPECT_CALL(*mockKeyRegister, get("A")).WillOnce(Return(0x1E));
    EXPECT_GT(KeyMapper::getKeyCode("A"), 0);
}

TEST_F(KeyMapperTest, GetKeyCode_InvalidKey_ThrowsException) {
    // Test that invalid keys throw an exception
    EXPECT_THROW(KeyMapper::getKeyCode("InvalidKey"), std::invalid_argument);
}

TEST_F(KeyMapperTest, GetModifierCode_ValidModifier_ReturnsNonZeroCode) {
    // Test that valid modifiers return a non-zero modifier code
    EXPECT_CALL(*mockKeyRegister, hasKey("CTRL")).WillOnce(Return(true));
    EXPECT_CALL(*mockKeyRegister, get("CTRL")).WillOnce(Return(0x1D));
    EXPECT_GT(KeyMapper::getModifierCode("Ctrl"), 0);
}

TEST_F(KeyMapperTest, GetModifierCode_InvalidModifier_ThrowsException) {
    // Test that invalid modifiers throw an exception
    EXPECT_THROW(KeyMapper::getModifierCode("InvalidModifier"), std::invalid_argument);
} 