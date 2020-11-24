#pragma once

#include <SDL.h>

#include <string_view>

namespace IceSDK::Graphics
{
    /*****************************************************
     * GameWindow
     *
     * Create a cross-platform window to Render graphics
     * onto it.
     *
     *****************************************************/
    class GameWindow
    {
    private:
        SDL_Window* m_SDLWindow = nullptr;

        int32_t m_iWidth = 0;
        int32_t m_iHeight = 0;

        bool m_bExit = false;

    public:
        enum Flags
        {
            None = 0,
            Fullscreen = 1 << 1
        };

        /*****************************************************
         * GameWindow
         *
         * Create a cross-platform window to Render graphics
         * onto it.
         *
         * NOTE: It must be created on the Render thread!
         *
         * @param iWidth Window Width
         * @param iHeight Window Height
         * @param svTitle Window Title
         * @param eFlags Window Flags
         *****************************************************/
        GameWindow(const int32_t iWidth = 800, const int32_t iHeight = 600,
                   const std::string_view svTitle = "IceSDK: Game Window",
                   const Flags eFlags = Flags::None);

        /*****************************************************
         * ~GameWindow
         *
         * Destroys the Window && Shutsdown BGFX
         *****************************************************/
        ~GameWindow();

        int32_t Width() { return m_iWidth; }
        int32_t Height() { return m_iHeight; }

        /*****************************************************
         * Tick
         *
         * Ticks the current state && Flushes the current
         * buffer
         *
         * NOTE: This must be called on the Render thread.
         *
         * @param iWidth Window Width
         * @param iHeight Window Height
         * @param svTitle Window Title
         * @param eFlags Window Flags
         *****************************************************/
        void Tick();

        /*****************************************************
         * ShouldExit
         *
         * Checks if this window should exit
         * cause E.G Exit() was called
         *
         * @return true if exit
         *****************************************************/
        bool ShouldExit();
    };
}  // namespace IceSDK::Graphics
