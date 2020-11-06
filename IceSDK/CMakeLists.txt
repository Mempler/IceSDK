add_library(IceSDK STATIC
        GameBase.cc

        #######################################################
        #                      Assets                         #
        #######################################################
        Assets/AssetFile.cc
        Assets/AssetManager.cc
        Assets/AudioAsset.cc
        Assets/ShaderAsset.cc
        Assets/TextAsset.cc
        Assets/Texture2DAsset.cc

        #######################################################
        #                       Audio                         #
        #######################################################
        Audio/Sound.cc
        Audio/AudioSystem.cc

        #######################################################
        #                        ECS                          #
        #######################################################
        ECS/Entity.cc
        ECS/EntityExtension.cc
        ECS/Scene.cc
        ECS/System.cc

        ECS/Systems/CameraSystem.cc
        ECS/Systems/TransformSystem.cc

        #######################################################
        #                     Graphics                        #
        #######################################################
        Graphics/Window.cc
        Graphics/Texture2D.cc

        Graphics/Fonts/FontFace.cc
        Graphics/Fonts/FontManager.cc

        Graphics/Shaders/ShaderManager.cc

        Graphics/SpriteBatch/SpriteBatch.cc

        Graphics/Systems/SpriteRenderingSystem.cc
        Graphics/Systems/TextRenderingSystem.cc

        Graphics/ImGui/bgfx_imgui.cc
        #       ImGui/Widgets/AssetBrowser.cc
        Graphics/ImGui/Widgets/SceneGraph.cc

        #######################################################
        #                       Input                         #
        #######################################################
        Input/InputPipeline.cc
        Input/Systems/MouseInputSystem.cc
        Input/Systems/KeyboardInputSystem.cc

        #######################################################
        #                       Utils                         #
        #######################################################
        Utils/FileSystem.cc
        Utils/Logger.cc
)

target_link_libraries(IceSDK PUBLIC bx bimg bgfx entt glm spdlog freetype imgui harfbuzz)
target_precompile_headers(IceSDK PUBLIC pch.h)
target_include_directories(IceSDK PUBLIC .)

if (ICESDK_USE_SDL2)
    target_link_libraries(IceSDK PUBLIC SDL2-static)
    target_include_directories(IceSDK PUBLIC ${SDL2_SOURCE_DIR}/include)
    target_compile_definitions(IceSDK PUBLIC -DICESDK_SDL2=1)

    if (UNIX)
        target_compile_definitions(IceSDK PUBLIC -DSDL_VIDEO_DRIVER_X11=1)
    endif()
endif()

if (ICESDK_USE_FMOD)
    target_link_libraries(IceSDK PUBLIC fmod)
    target_compile_definitions(IceSDK PUBLIC -DICESDK_FMOD=1)
endif()

if (ICESDK_USE_PROFILER)
    target_compile_definitions(IceSDK PUBLIC -DICESDK_PROFILE=1)
endif()
