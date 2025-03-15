#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <nlohmann/json.hpp>

#include "window/component/message/resize/resize_message_mapper.hpp"
#include "window/component/message/resize/resize_message_vo.hpp"

using namespace palantir::window::component::message::resize;
using namespace testing;

class ResizeMessageMapperTest : public Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(ResizeMessageMapperTest, FromJson_ValidInput_ReturnsResizeMessageVO) {
    nlohmann::json json = {
        {"width", 800},
        {"height", 600}
    };

    auto result = ResizeMessageMapper::fromJson(json);
    EXPECT_EQ(result.width, 800);
    EXPECT_EQ(result.height, 600);
}

TEST_F(ResizeMessageMapperTest, FromJson_ZeroValues_ReturnsResizeMessageVO) {
    nlohmann::json json = {
        {"width", 0},
        {"height", 0}
    };

    auto result = ResizeMessageMapper::fromJson(json);
    EXPECT_EQ(result.width, 0);
    EXPECT_EQ(result.height, 0);
}

TEST_F(ResizeMessageMapperTest, FromJson_NegativeValues_ReturnsResizeMessageVO) {
    nlohmann::json json = {
        {"width", -100},
        {"height", -200}
    };

    auto result = ResizeMessageMapper::fromJson(json);
    EXPECT_EQ(result.width, -100);
    EXPECT_EQ(result.height, -200);
}

TEST_F(ResizeMessageMapperTest, FromJson_MissingWidth_ThrowsException) {
    nlohmann::json json = {
        {"height", 600}
    };

    EXPECT_THROW(ResizeMessageMapper::fromJson(json), std::invalid_argument);
}

TEST_F(ResizeMessageMapperTest, FromJson_MissingHeight_ThrowsException) {
    nlohmann::json json = {
        {"width", 800}
    };

    EXPECT_THROW(ResizeMessageMapper::fromJson(json), std::invalid_argument);
}

TEST_F(ResizeMessageMapperTest, FromJson_EmptyJson_ThrowsException) {
    nlohmann::json json = nlohmann::json::object();
    EXPECT_THROW(ResizeMessageMapper::fromJson(json), std::invalid_argument);
}

TEST_F(ResizeMessageMapperTest, FromJson_InvalidTypes_ThrowsException) {
    nlohmann::json json = {
        {"width", "800"},  // String instead of int
        {"height", 600}
    };

    EXPECT_THROW(ResizeMessageMapper::fromJson(json), nlohmann::json::type_error);
} 