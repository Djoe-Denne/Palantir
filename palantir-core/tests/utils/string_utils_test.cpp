#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "utils/string_utils.hpp"
#include <vector>
#include <string>

using namespace palantir::utils;

class StringUtilsTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(StringUtilsTest, ToUpperEmptyString) {
    EXPECT_EQ(StringUtils::toUpper(""), "");
}

TEST_F(StringUtilsTest, ToUpperLowerCase) {
    EXPECT_EQ(StringUtils::toUpper("hello"), "HELLO");
}

TEST_F(StringUtilsTest, ToUpperMixedCase) {
    EXPECT_EQ(StringUtils::toUpper("HeLLo WoRLD"), "HELLO WORLD");
}

TEST_F(StringUtilsTest, ToUpperWithNumbers) {
    EXPECT_EQ(StringUtils::toUpper("hello123"), "HELLO123");
}

TEST_F(StringUtilsTest, ToUpperWithSpecialChars) {
    EXPECT_EQ(StringUtils::toUpper("hello!@#$%"), "HELLO!@#$%");
}

TEST_F(StringUtilsTest, Base64EncodeEmptyVector) {
    std::vector<unsigned char> empty;
    EXPECT_EQ(StringUtils::base64_encode(empty), "");
}

TEST_F(StringUtilsTest, Base64EncodeSingleByte) {
    std::vector<unsigned char> data{'M'};
    EXPECT_EQ(StringUtils::base64_encode(data), "TQ==");
}

TEST_F(StringUtilsTest, Base64EncodeTwoBytes) {
    std::vector<unsigned char> data{'M', 'a'};
    EXPECT_EQ(StringUtils::base64_encode(data), "TWE=");
}

TEST_F(StringUtilsTest, Base64EncodeThreeBytes) {
    std::vector<unsigned char> data{'M', 'a', 'n'};
    EXPECT_EQ(StringUtils::base64_encode(data), "TWFu");
}

TEST_F(StringUtilsTest, Base64EncodeLongString) {
    std::vector<unsigned char> data{'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd'};
    EXPECT_EQ(StringUtils::base64_encode(data), "SGVsbG8gV29ybGQ=");
}

TEST_F(StringUtilsTest, Base64EncodeSpecialChars) {
    std::vector<unsigned char> data{0xFF, 0x00, 0xAA};
    EXPECT_EQ(StringUtils::base64_encode(data), "/wCq");
}

TEST_F(StringUtilsTest, Base64EncodePadding) {
    std::vector<unsigned char> data1{'f'};
    std::vector<unsigned char> data2{'f', 'o'};
    std::vector<unsigned char> data3{'f', 'o', 'o'};
    
    EXPECT_EQ(StringUtils::base64_encode(data1), "Zg==");
    EXPECT_EQ(StringUtils::base64_encode(data2), "Zm8=");
    EXPECT_EQ(StringUtils::base64_encode(data3), "Zm9v");
} 