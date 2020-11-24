#pragma once

#include "IceSDK/Assets/AssetManager.hh"
#include "IceSDK/Audio/AudioEngine.hh"
#include "IceSDK/Graphics/FontManager.hh"
#include "IceSDK/Graphics/Shaders/ShaderManager.hh"
#include "IceSDK/Graphics/SpriteBatch.hh"
#include "IceSDK/Graphics/Window.hh"
#include "IceSDK/Input/InputPipeline.hh"
#include "IceSDK/Scene.hh"

#include <bx/allocator.hh>

namespace IceSDK
{
    class GameBase
    {
    public:
        explicit GameBase();
        ~GameBase();

        void Run();

        Audio::AudioSystem* GetAudioSystem() const;
        Graphics::SpriteBatch* GetSpriteBatch() const;
        Assets::AssetManager* GetAssetManager() const;
        Graphics::GameWindow* GetGameWindow() const;
        Graphics::Shaders::ShaderManager* GetShaderManager() const;
        Graphics::FontManager* GetFontManager() const;
        Input::InputPipeline* GetInputPipeline() const;
        std::weak_ptr<ECS::Scene> GetActiveScene() const;

    protected:
        virtual void Init();
        virtual void Shutdown();

        virtual void Update(float pDelta);
        virtual void Draw(float pDelta);
        virtual void InitDraw();

        ECS::Scene* _active_scene;

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

        Graphics::GameWindow* _window;
        Audio::AudioSystem* _audio_system;
        Graphics::SpriteBatch* _sprite_batch;
        Assets::AssetManager* _asset_manager;
        Graphics::Shaders::ShaderManager* _shader_manager;
        Graphics::FontManager* _font_manager;
        Input::InputPipeline* _input_pipeline;
    };
}  // namespace IceSDK

extern IceSDK::GameBase* GetGameBase();

#if defined(ICESDK_SDL2) && defined(ICESDK_ANDROID)
    #define IceSDKMain IceSDK_main
#else
    #undef main
    #define IceSDKMain main
#endif
