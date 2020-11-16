add_subdirectory(Components/Core)
add_subdirectory(Components/Scripting)
add_subdirectory(Components/ScriptBindings)

add_library(IceSDK INTERFACE)
target_link_libraries(IceSDK INTERFACE IceSDKCore IceSDKScripting IceSDKBindings)

add_executable(Game main.cc)
target_link_libraries(Game PUBLIC IceSDK)
