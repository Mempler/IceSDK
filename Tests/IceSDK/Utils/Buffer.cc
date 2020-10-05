#include "Utils/Buffer.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

TEST(Buffer, PushI8)
{
    IceSDK::Buffer buffer;

    buffer.push<int8_t>(0x0F);

    ASSERT_THAT(buffer.data(), testing::ElementsAre(0x0F));
}

TEST(Buffer, PushI16)
{
    IceSDK::Buffer buffer;

    buffer.push<int16_t>(0xFFFA);

    ASSERT_THAT(buffer.data(), testing::ElementsAre(0xFA, 0xFF));
}

TEST(Buffer, PushI32)
{
    IceSDK::Buffer buffer;

    buffer.push<int32_t>(0xFFFFFFFA);

    ASSERT_THAT(buffer.data(), testing::ElementsAre(0xFA, 0xFF, 0xFF, 0xFF));
}

TEST(Buffer, PushI64)
{
    IceSDK::Buffer buffer;

    buffer.push<int64_t>(0xFFFFFFFFFFFFFFFA);

    ASSERT_THAT(buffer.data(), testing::ElementsAre(0xFA, 0xFF, 0xFF, 0xFF,
                                                    0xFF, 0xFF, 0xFF, 0xFF));
}

TEST(Buffer, PushU8)
{
    IceSDK::Buffer buffer;

    buffer.push<uint8_t>(0x0F);

    ASSERT_THAT(buffer.data(), testing::ElementsAre(0x0F));
}

TEST(Buffer, PushU16)
{
    IceSDK::Buffer buffer;

    buffer.push<uint16_t>(0xFFFA);

    ASSERT_THAT(buffer.data(), testing::ElementsAre(0xFA, 0xFF));
}

TEST(Buffer, PushU32)
{
    IceSDK::Buffer buffer;

    buffer.push<uint32_t>(0xFFFFFFFA);

    ASSERT_THAT(buffer.data(), testing::ElementsAre(0xFA, 0xFF, 0xFF, 0xFF));
}

TEST(Buffer, PushU64)
{
    IceSDK::Buffer buffer;

    buffer.push<uint64_t>(0xFFFFFFFFFFFFFFFA);

    ASSERT_THAT(buffer.data(), testing::ElementsAre(0xFA, 0xFF, 0xFF, 0xFF,
                                                    0xFF, 0xFF, 0xFF, 0xFF));
}

TEST(Buffer, PushFloat)
{
    IceSDK::Buffer buffer;

    buffer.push<float>(0.12345f);

    ASSERT_THAT(buffer.data(), testing::ElementsAre(0x5B, 0xD3, 0xFC, 0x3D));
}

TEST(Buffer, PushDouble)
{
    IceSDK::Buffer buffer;

    buffer.push<double>(0.12345);

    ASSERT_THAT(buffer.data(), testing::ElementsAre(0x7C, 0xF2, 0xB0, 0x50,
                                                    0x6B, 0x9A, 0xBF, 0x3F));
}

TEST(Buffer, PushCString)
{
    IceSDK::Buffer buffer;

    buffer.push<const char*>("Hello World!");

    ASSERT_THAT(buffer.data(),
                testing::ElementsAre('H', 'e', 'l', 'l', 'o', ' ', 'W', 'o',
                                     'r', 'l', 'd', '!'));
}

TEST(Buffer, PushCPPString)
{
    IceSDK::Buffer buffer;

    buffer.push<std::string>("Hello World!");

    ASSERT_THAT(buffer.data(),
                testing::ElementsAre('H', 'e', 'l', 'l', 'o', ' ', 'W', 'o',
                                     'r', 'l', 'd', '!'));
}

TEST(Buffer, PopI8)
{
    IceSDK::Buffer buffer;

    buffer.push<int8_t>(0x0F);

    ASSERT_EQ(buffer.pop<int8_t>(1), 0x0F);
}

TEST(Buffer, PopI16)
{
    IceSDK::Buffer buffer;

    buffer.push<int16_t>(0xFFFA);

    ASSERT_EQ(buffer.pop<int16_t>(2), -6);
}

TEST(Buffer, PopI32)
{
    IceSDK::Buffer buffer;

    buffer.push<int32_t>(0xFFFFFFFA);

    ASSERT_EQ(buffer.pop<int32_t>(4), 0xFFFFFFFA);
}

TEST(Buffer, PopI64)
{
    IceSDK::Buffer buffer;

    buffer.push<int64_t>(0xFFFFFFFFFFFFFFFA);

    ASSERT_EQ(buffer.pop<int64_t>(8), 0xFFFFFFFFFFFFFFFA);
}

TEST(Buffer, PopU8)
{
    IceSDK::Buffer buffer;

    buffer.push<uint8_t>(0x0F);

    ASSERT_EQ(buffer.pop<uint8_t>(1), 0x0F);
}

TEST(Buffer, PopU16)
{
    IceSDK::Buffer buffer;

    buffer.push<uint16_t>(0xFFFA);

    ASSERT_EQ(buffer.pop<uint16_t>(2), 0xFFFA);
}

TEST(Buffer, PopU32)
{
    IceSDK::Buffer buffer;

    buffer.push<uint32_t>(0xFFFFFFFA);

    ASSERT_EQ(buffer.pop<uint32_t>(4), 0xFFFFFFFA);
}

TEST(Buffer, PopU64)
{
    IceSDK::Buffer buffer;

    buffer.push<uint64_t>(0xFFFFFFFFFFFFFFFA);

    ASSERT_EQ(buffer.pop<uint64_t>(8), 0xFFFFFFFFFFFFFFFA);
}

TEST(Buffer, PopFloat)
{
    IceSDK::Buffer buffer;

    buffer.push<float>(0.12345f);

    ASSERT_EQ(buffer.pop<float>(4), 0.12345f);
}

TEST(Buffer, PopDouble)
{
    IceSDK::Buffer buffer;

    buffer.push<double>(0.12345);

    ASSERT_EQ(buffer.pop<double>(8), 0.12345);
}

TEST(Buffer, PopCString)
{
    IceSDK::Buffer buffer;

    buffer.push<const char*>("Hello World!");

    ASSERT_STREQ(buffer.pop<const char*>(12), "Hello World!");
}

TEST(Buffer, PopCPPString)
{
    IceSDK::Buffer buffer;

    buffer.push<std::string>("Hello World!");

    ASSERT_EQ(buffer.pop<std::string>(12), "Hello World!");
}
