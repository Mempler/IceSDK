add_library(IceSDK STATIC
        src/GameBase.cc
        src/Entity.cc
        src/Scene.cc

        #######################################################
        #                      Systems                        #
        #######################################################

        src/Systems/CameraSystem.cc
        src/Systems/TransformSystem.cc
        src/Systems/SpriteRenderingSystem.cc
        src/Systems/TextRenderingSystem.cc
        src/Systems/MouseInputSystem.cc
        src/Systems/KeyboardInputSystem.cc

        #######################################################
        #                      Assets                         #
        #######################################################
        src/Assets/AssetFile.cc
        src/Assets/AssetManager.cc
        src/Assets/AudioAsset.cc
        src/Assets/ShaderAsset.cc
        src/Assets/TextAsset.cc
        src/Assets/Texture2DAsset.cc

        #######################################################
        #                       Audio                         #
        #######################################################
        src/Audio/Sound.cc
        src/Audio/AudioEngine.cc

        #######################################################
        #                     Graphics                        #
        #######################################################
        src/Graphics/Window.cc
        src/Graphics/Texture2D.cc
        src/Graphics/SpriteBatch.cc
        src/Graphics/FontFace.cc
        src/Graphics/FontManager.cc

        src/Graphics/Shaders/ShaderManager.cc


        src/Graphics/ImGui/bgfx_imgui.cc
        src/Graphics/ImGui/Widgets/AssetBrowser.cc
        src/Graphics/ImGui/Widgets/SceneGraph.cc

        #######################################################
        #                       Input                         #
        #######################################################
        src/Input/InputPipeline.cc
)

target_link_libraries(IceSDK PUBLIC bx bimg bgfx entt glm spdlog freetype imgui harfbuzz)
target_precompile_headers(IceSDK PUBLIC src/pch.hh)
target_include_directories(IceSDK PUBLIC include PRIVATE src)

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
