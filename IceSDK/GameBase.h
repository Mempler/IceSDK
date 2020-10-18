#pragma once

#include "Assets/AssetManager.h"
#include "Audio/AudioSystem.h"
#include "ECS/Scene.h"

#include "Input/InputPipeline.h"
#include "Utils/Memory.h"

#include "Graphics/Fonts/FontManager.h"
#include "Graphics/Shaders/ShaderManager.h"
#include "Graphics/SpriteBatch/SpriteBatch.h"
#include "Graphics/Window.h"

#include <bx/allocator.h>

#include <thread>

namespace IceSDK
{
    class GameBase
    {
    public:
        explicit GameBase();
        ~GameBase();

        void Run();

        Memory::Ptr<Audio::AudioSystem> GetAudioSystem() const;
        Memory::Ptr<Graphics::SpriteBatch> GetSpriteBatch() const;
        Memory::Ptr<Assets::AssetManager> GetAssetManager() const;
        Memory::Ptr<Graphics::GameWindow> GetGameWindow() const;
        Memory::Ptr<Graphics::Shaders::ShaderManager> GetShaderManager() const;
        Memory::Ptr<Graphics::FontManager> GetFontManager() const;
        Memory::Ptr<Input::InputPipeline> GetInputPipeline() const;
        Memory::WeakPtr<Scene> GetActiveScene() const;

    protected:
        virtual void Init();
        virtual void Shutdown();

        virtual void Update(float pDelta);
        virtual void Draw(float pDelta);
        virtual void InitDraw();

        Memory::Ptr<Scene> _active_scene;

    private:
        static void InternalDraw(float pDelta);
        static void InternalTick(float pDelta);
        static void InternalDrawInit();
        static void InternalShutdown();

        int64_t _last_delta;
#ifdef ICESDK_EMSCRIPTEN
        static void InternalMainLoop(void* arg);
#else
        bool _exit = false;
#endif

        Memory::Ptr<Graphics::GameWindow> _window;
        Memory::Ptr<Audio::AudioSystem> _audio_system;
        Memory::Ptr<Graphics::SpriteBatch> _sprite_batch;
        Memory::Ptr<Assets::AssetManager> _asset_manager;
        Memory::Ptr<Graphics::Shaders::ShaderManager> _shader_manager;
        Memory::Ptr<Graphics::FontManager> _font_manager;
        Memory::Ptr<Input::InputPipeline> _input_pipeline;
    };
}  // namespace IceSDK

extern IceSDK::Memory::Ptr<IceSDK::GameBase> GetGameBase();
IceSDK::Memory::Ptr<IceSDK::Graphics::GameWindow> GetWindow();
IceSDK::Memory::Ptr<bx::AllocatorI> GetAllocator();
IceSDK::Memory::Ptr<IceSDK::Audio::AudioSystem> GetAudioSystem();

#if defined(ICESDK_SDL2) && defined(ICESDK_ANDROID)
    #define IceSDKMain IceSDK_main
#else
    #undef main
    #define IceSDKMain main
#endif
