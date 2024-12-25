#include <gtest/gtest.h>

#include <cml/cml.h>

TEST(SuiteName, TestName)
{
    auto obj = cml::Parse(u8"test|view|string:text|int:amount|int:multiplier");
    ulib::u8string command = u8"test view Some 281231 3120";

    auto match = cml::Match(command, obj);

    ASSERT_EQ(match.state, cml::MatchState::Success);
    ASSERT_TRUE(match.args.has_value());
    
    ASSERT_EQ(match.args->at(0), u8"Some");
    ASSERT_EQ(match.args->at(1), u8"281231");
    ASSERT_EQ(match.args->at(2), u8"3120");
}

TEST(SuiteName, VarArgs)
{
    auto obj = cml::Parse(u8"test|");
    ulib::u8string command = u8"test Some 281231 3120";

    auto match = cml::Match(command, obj);

    ASSERT_EQ(match.state, cml::MatchState::Success);
    ASSERT_TRUE(match.args.has_value());
    
    ASSERT_EQ(match.args->at(0), u8"Some");
    ASSERT_EQ(match.args->at(1), u8"281231");
    ASSERT_EQ(match.args->at(2), u8"3120");
}