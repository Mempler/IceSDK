#include "Utils/String.h"

#include "gtest/gtest.h"

TEST(String, Trim)
{
    std::string input = "    YO!    ";

    ASSERT_EQ(IceSDK::String::Trim(input, " "), "YO!");
}

Test(String, CalculateHash)
{
    std::string input = "Hello World!";

    ASSERT_EQ(IceSDK::String::CalculateHash(input, " "), "YO!");
}
