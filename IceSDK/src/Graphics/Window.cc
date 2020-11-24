#include "pch.h"

#include "Graphics/Window.h"

using namespace IceSDK;
using namespace IceSDK::Core;
using namespace IceSDK::Core::Graphics;

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
GameWindow::GameWindow(const int32_t iWidth, const int32_t iHeight,
                       const std::string_view svTitle, const Flags eFlags)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0)
        ICESDK_CORE_CRITICAL("Failed to initialize SDL2!");

#ifdef ICESDK_ANDROID
    SDL_SetHint(SDL_HINT_ORIENTATIONS, "LandscapeLeft LandscapeRight");
    m_SDLWindow = SDL_CreateWindow(pTitle.c_str(), SDL_WINDOWPOS_UNDEFINED,
                                   SDL_WINDOWPOS_UNDEFINED, 0, 0, 0);
#else
    int32_t flags = 0;
    if ((eFlags & Flags::Fullscreen) > 0) flags = SDL_WINDOW_FULLSCREEN;

    m_SDLWindow =
        SDL_CreateWindow(svTitle.data(), SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED, iWidth, iHeight, flags);
#endif

    if (m_SDLWindow == nullptr)
        ICESDK_CORE_CRITICAL("Failed to create window!");

    // create platform data for BGFX
    bgfx::PlatformData platformData;
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version)
    SDL_GetWindowWMInfo(m_SDLWindow, &wmInfo);

#if ICESDK_WIN32
    platformData.nwh = wmInfo.info.win.window;
#elif ICESDK_LINUX
    platformData.ndt = wmInfo.info.x11.display;
    platformData.nwh = (void*) (uintptr_t) wmInfo.info.x11.window;
#elif ICESDK_EMSCRIPTEN
    platformData.nwh = (void*) "#canvas";
#elif ICESDK_ANDROID
    platformData.ndt = 0;
    platformData.nwh = (void*) wmInfo.info.android.window;

    // Delete existing surface
    // TODO: use EGL for Wayland on Linux
    eglDestroySurface(eglGetDisplay(EGL_DEFAULT_DISPLAY),
                      wmInfo.info.android.surface);
#else
    #error "Platform not implemented!"
#endif

    // We need those variables internally
    m_iWidth = iWidth;
    m_iHeight = iHeight;

    // Initialize BGFX using our Platform data
    bgfx::Init bgfxInit;
    // bgfxInit.debug = true; // This slows down the renderer quite a bit

    bgfxInit.type =
        bgfx::RendererType::Count;  // Automatically choose a renderer.
    bgfxInit.platformData = platformData;
    bgfxInit.resolution.width = m_iWidth;
    bgfxInit.resolution.height = m_iHeight;
    bgfxInit.resolution.reset = BGFX_RESET_NONE;
    if (!bgfx::init(bgfxInit))
        ICESDK_CORE_CRITICAL("Failed to initialize BGFX!");

    ICESDK_CORE_TRACE("Game Window Initialized...");
    ICESDK_CORE_TRACE("BGFX Initialized...");

    // Setup our drawing surface
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x000000FF, 1.0f,
                       0);

    bgfx::setViewRect(0, 0, 0, bgfx::BackbufferRatio::Equal);

#ifdef ICESDK_ANDROID
    // Set display size
    SDL_DisplayMode displayMode;
    SDL_GetDesktopDisplayMode(0, &displayMode);

    _width = static_cast<uint32_t>(displayMode.w);
    _height = static_cast<uint32_t>(displayMode.h);

    bgfx::reset(_width, _height, BGFX_RESET_NONE);
    bgfx::setViewRect(0, 0, 0, bgfx::BackbufferRatio::Equal);
#endif
}

/*****************************************************
 * ~GameWindow
 *
 * Destroys the Window && Shutsdown BGFX
 *****************************************************/
GameWindow::~GameWindow()
{
    // Shutdown BGFX
    bgfx::shutdown();

    // Shutdown SDL2
    SDL_DestroyWindow(m_SDLWindow);
    SDL_Quit();
}

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
void GameWindow::Tick()
{
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0)
    {  // TODO: Event Pipeline
        switch (e.type)
        {
        // Tell our window that we shall exit now
        case SDL_QUIT: m_bExit = true; break;
        case SDL_WINDOWEVENT:
            if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
            {
                ICESDK_CORE_INFO("Window size changed...");

                SDL_DisplayMode displayMode;
                SDL_GetDesktopDisplayMode(0, &displayMode);

                m_iWidth = displayMode.w;
                m_iHeight = displayMode.h;

                bgfx::reset(m_iWidth, m_iHeight, BGFX_RESET_NONE);
                bgfx::setViewRect(0, 0, 0, bgfx::BackbufferRatio::Equal);
            }
            break;
        default: break;
        }
    }

    /*
    const auto now = bx::getHPCounter();
    const auto frameTime = now - _last_time;
    _last_time = now;

    const auto freq = static_cast<float>(bx::getHPFrequency());
    const auto delta = static_cast<float>(frameTime) / freq;
    */

    // This dummy draw call is here to make sure that view 0 is cleared if no
    // other draw calls are submitted to view 0.
    bgfx::touch(0);
    // TODO :Draw
    bgfx::frame();
}

/*****************************************************
 * ShouldExit
 *
 * Checks if this window should exit
 * cause E.G Exit() was called
 *
 * @return true if exit
 *****************************************************/
bool GameWindow::ShouldExit()
{
    return m_bExit;
}
