add_executable(TestIceSDK
        Test.cc

        IceSDK/Utils/Buffer.cc
        IceSDK/Utils/FileSystem.cc
        IceSDK/Utils/String.cc)

target_link_libraries(TestIceSDK IceSDK gtest gtest_main gmock)
target_include_directories(TestIceSDK PUBLIC
        ../third-party/googletest/googletest/include
        ../third-party/googletest/googlemock/include)
