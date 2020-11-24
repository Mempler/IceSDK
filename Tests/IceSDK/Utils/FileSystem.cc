#include "Utils/FileSystem.h"

#include "Utils/Logger.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

TEST(FileSystem, MkDir)
{
    IceSDK::Log::Init();
    IceSDK::FileSystem::MkDir("./TestDir");

    ASSERT_TRUE(IceSDK::FileSystem::Exists("./TestDir"));
    ASSERT_FALSE(IceSDK::FileSystem::Exists("./TestDir2"));
}

TEST(FileSystem, Exists)
{
    IceSDK::Log::Init();
    IceSDK::FileSystem::MkDir("./TestDir");

    ASSERT_TRUE(IceSDK::FileSystem::Exists("./TestDir"));
    ASSERT_FALSE(IceSDK::FileSystem::Exists("./TestDir2"));
}

TEST(FileSystem, JoinPath)
{
    IceSDK::Log::Init();
    ASSERT_EQ("./test/dir", IceSDK::FileSystem::JoinPath("./test/", "dir"));
}

TEST(FileSystem, IsDirectory)
{
    IceSDK::Log::Init();
    IceSDK::FileSystem::MkDir("./TestDir");
    IceSDK::FileSystem::Touch("./TestFile");

    ASSERT_TRUE(IceSDK::FileSystem::IsDirectory("./TestDir"));
    ASSERT_FALSE(IceSDK::FileSystem::IsDirectory("./TestDir2"));
    ASSERT_FALSE(IceSDK::FileSystem::IsDirectory("./TestFile"));
}

TEST(FileSystem, ReadDirectory)
{
    IceSDK::Log::Init();
    IceSDK::FileSystem::MkDir("./TestDir");
    IceSDK::FileSystem::MkDir("./TestDir/Dir");
    IceSDK::FileSystem::Touch("./TestDir/File");

    auto dir_info = IceSDK::FileSystem::ReadDirectory("./TestDir");

    ASSERT_THAT(dir_info,
                testing::ElementsAre("./TestDir/Dir", "./TestDir/File"));
}

TEST(FileSystem, ResolveFullPath)
{
    IceSDK::Log::Init();
    ASSERT_NE(IceSDK::FileSystem::ResolveFullPath("."),
              "");  // there is no real way we could test it, other than it's
                    // not being empty
}

TEST(FileSystem, ReadBinaryFile)
{
    IceSDK::Log::Init();
    std::vector<uint8_t> data = { 'M', 'e', 'm', 'p', 'l', 'e', 'r', 17 };

    IceSDK::FileSystem::WriteBinaryFile("./TestFile", data);

    ASSERT_THAT(IceSDK::FileSystem::ReadBinaryFile("./TestFile"),
                testing::ElementsAre('M', 'e', 'm', 'p', 'l', 'e', 'r', 17));
}

TEST(FileSystem, WriteBinaryFile)
{
    IceSDK::Log::Init();
    std::vector<uint8_t> data = { 'M', 'e', 'm', 'p', 'l', 'e', 'r', 17 };

    IceSDK::FileSystem::WriteBinaryFile("./TestFile", data);

    ASSERT_THAT(IceSDK::FileSystem::ReadBinaryFile("./TestFile"),
                testing::ElementsAre('M', 'e', 'm', 'p', 'l', 'e', 'r', 17));
}

TEST(FileSystem, HasExtension)
{
    IceSDK::Log::Init();
    IceSDK::FileSystem::Touch("./TestFile.test");

    auto testFile = (const char*) malloc(16);
    memcpy((void*) testFile, "./TestFile.test", 16);

    ASSERT_TRUE(IceSDK::FileSystem::HasExtension(testFile, ".test"));
    ASSERT_TRUE(IceSDK::FileSystem::HasExtension("./TestFile.test", ".test"));
    ASSERT_FALSE(IceSDK::FileSystem::HasExtension("./TestFile.test", ".est"));
}

TEST(FileSystem, GetFileName)
{
    IceSDK::Log::Init();
    IceSDK::FileSystem::Touch("./TestFile");

    auto testFile = (const char*) malloc(11);
    memcpy((void*) testFile, "./TestFile", 11);

    ASSERT_EQ(IceSDK::FileSystem::GetFileName(testFile), "TestFile");
    ASSERT_EQ(IceSDK::FileSystem::GetFileName("TestFile"), "TestFile");
    ASSERT_EQ(IceSDK::FileSystem::GetFileName("./TestFile"), "TestFile");
    ASSERT_EQ(IceSDK::FileSystem::GetFileName("./test/TestFile"), "TestFile");
}

TEST(FileSystem, Touch)
{
    IceSDK::Log::Init();
    IceSDK::FileSystem::Touch("./TestFile");

    ASSERT_TRUE(IceSDK::FileSystem::Exists("./TestFile"));
    ASSERT_FALSE(IceSDK::FileSystem::Exists("./TestFile2"));
}
