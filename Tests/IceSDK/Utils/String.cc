#include "Utils/String.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

TEST(String, Trim)
{
    std::string input = "    YO!    ";

    ASSERT_EQ(IceSDK::String::Trim(input, " "), "YO!");
}
