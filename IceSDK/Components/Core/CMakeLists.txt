add_library(
    IceSDKCore STATIC

    ECS/Entity.cc
    ECS/Scene.cc
    ECS/System.cc

    Graphics/Window.cc
    Graphics/Texture2D.cc

    Utils/FileSystem.cc
    Utils/Logger.cc
)

target_link_libraries(IceSDKCore PUBLIC
    bgfx bx bimg SDL2-static spdlog entt)
target_include_directories(IceSDKCore
    PUBLIC ${SDL2_SOURCE_DIR}/include
    PRIVATE .
    PUBLIC ..)

if (UNIX)
    target_compile_definitions(IceSDKCore PUBLIC -DSDL_VIDEO_DRIVER_X11=1)
endif()
