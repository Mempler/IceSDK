#pragma once

#include <bx/thread.h>
#include <bx/timer.h>

#include <cstdint>
#include <functional>
#include <string>

#if defined(ICESDK_SDL2)
    #include <SDL.h>
#endif

namespace IceSDK
{
    class GameBase;

    using DrawCallback_t = std::function<void(float)>;
    using DrawInitCallback_t = std::function<void()>;

    namespace Graphics
    {
        enum eGameWindowFlags
        {
            None = 0,
            Fullscreen = 1 << 1
        };

        class GameWindow
        {
        public:
            explicit GameWindow(
                int32_t pWidth, int32_t pHeight, const std::string& pTitle,
                eGameWindowFlags pFlags = eGameWindowFlags::None);
            ~GameWindow();

            void Update();

            void SetDrawCallback(DrawCallback_t pCallback);
            void SetDrawInitCallback(DrawInitCallback_t pCallback);

            // true = Window closed
            bool ShouldClose() const;

            uint32_t Width() const;
            uint32_t Height() const;

#if defined(ICESDK_SDL2)
            SDL_Window* GetNativeWindow() { return this->_window; }
#endif

        private:
            friend GameBase;

#if defined(ICESDK_SDL2)
            SDL_Window* _window = nullptr;

            bool _should_exit = false;
#endif

            DrawCallback_t _draw_callback = nullptr;
            DrawInitCallback_t _draw_init_callback = nullptr;
            uint32_t _width{}, _height{};
            bool _initialized = false;

            int64_t _last_time = bx::getHPCounter();
        };
    }  // namespace Graphics
}  // namespace IceSDK
