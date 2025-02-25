#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "command/commands_plugin.hpp"

using namespace interview_cheater::plugins;
using namespace testing;

class CommandsPluginTest : public Test {
protected:
    CommandsPlugin plugin_;
};

TEST_F(CommandsPluginTest, Initialize_ReturnsTrue) {
    EXPECT_TRUE(plugin_.initialize());
}

TEST_F(CommandsPluginTest, Shutdown_DoesNotThrow) {
    EXPECT_NO_THROW(plugin_.shutdown());
}

TEST_F(CommandsPluginTest, GetName_ReturnsNonEmptyString) {
    EXPECT_THAT(plugin_.getName(), Not(IsEmpty()));
}

TEST_F(CommandsPluginTest, GetVersion_ReturnsNonEmptyString) {
    EXPECT_THAT(plugin_.getVersion(), Not(IsEmpty()));
} 