#include "pch.h"

#include "Graphics/Window.h"

#include "Utils/Instrumentor.h"
#include "Utils/Logger.h"

#include "GameBase.h"

using namespace IceSDK::Graphics;

#ifdef ICESDK_GLFW
void GameWindow::ResizeGameWindow(GLFWwindow* pWindow, const int pWidth,
                                  const int pHeight)
{
    ICESDK_PROFILE_FUNCTION();
    auto* self = static_cast<GameWindow*>(glfwGetWindowUserPointer(pWindow));

    self->_width = static_cast<uint32_t>(pWidth);
    self->_height = static_cast<uint32_t>(pHeight);

    bgfx::reset(self->_width, self->_height, BGFX_RESET_NONE);
    bgfx::setViewRect(0, 0, 0, bgfx::BackbufferRatio::Equal);

    self->Update();
}
#endif

GameWindow::GameWindow(int32_t pWidth, int32_t pHeight,
                       const std::string& pTitle, const eGameWindowFlags pFlags)
{
    ICESDK_PROFILE_FUNCTION();

#ifdef ICESDK_GLFW
    if (!glfwInit()) ICESDK_CORE_CRITICAL("Failed to initialize GLFW!");

    GLFWmonitor* monitor = nullptr;
    if (pFlags & eGameWindowFlags::Fullscreen)
        monitor = glfwGetPrimaryMonitor();

    // glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    this->_window =
        glfwCreateWindow(pWidth, pHeight, pTitle.c_str(), monitor, nullptr);
    if (this->_window == nullptr)
        ICESDK_CORE_CRITICAL("Failed to create window!");

    glfwSetWindowUserPointer(this->_window, this);
    glfwSetWindowSizeCallback(this->_window, &GameWindow::ResizeGameWindow);
#elif defined(ICESDK_SDL2)
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0)
        ICESDK_CORE_CRITICAL("Failed to initialize SDL2!");

    #ifdef ICESDK_ANDROID
    SDL_SetHint(SDL_HINT_ORIENTATIONS, "LandscapeLeft LandscapeRight");
    this->_window = SDL_CreateWindow(pTitle.c_str(), SDL_WINDOWPOS_UNDEFINED,
                                     SDL_WINDOWPOS_UNDEFINED, 0, 0, 0);
    #else
    this->_window =
        SDL_CreateWindow(pTitle.c_str(), SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED, pWidth, pHeight, 0);
    #endif

    if (this->_window == nullptr)
        ICESDK_CORE_CRITICAL("Failed to create window!");
#endif

    bgfx::PlatformData platformData;
#ifdef ICESDK_GLFW
    #ifdef ICESDK_WIN32
    platformData.nwh = glfwGetWin32Window(this->_window);
    #elif defined(ICESDK_LINUX)
    platformData.nwh = (void*) glfwGetX11Window(this->_window);
    #else
        #error "Platform not implemented!"
    #endif
#elif defined(ICESDK_SDL2)
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version)
    SDL_GetWindowWMInfo(this->_window, &wmInfo);

    #ifdef ICESDK_WIN32
    platformData.nwh = wmInfo.info.win.window;
    #elif defined(ICESDK_LINUX)
    platformData.ndt = wmInfo.info.x11.display;
    platformData.nwh = (void*) (uintptr_t) wmInfo.info.x11.window;
    #elif defined(ICESDK_EMSCRIPTEN)
    platformData.nwh = (void*) "#canvas";
    #elif defined(ICESDK_ANDROID)
    platformData.ndt = 0;
    platformData.nwh = (void*) wmInfo.info.android.window;

    // Delete existing surface
    eglDestroySurface(eglGetDisplay(EGL_DEFAULT_DISPLAY),
                      wmInfo.info.android.surface);
    #else
        #error "Platform not implemented!"
    #endif

#endif

    this->_width = static_cast<uint32_t>(pWidth);
    this->_height = static_cast<uint32_t>(pHeight);

    bgfx::Init bgfxInit;
    bgfxInit.debug = true;
    bgfxInit.type =
        bgfx::RendererType::Count;  // Automatically choose a renderer.
    bgfxInit.platformData = platformData;
    bgfxInit.resolution.width = pWidth;
    bgfxInit.resolution.height = pHeight;
    bgfxInit.resolution.reset = BGFX_RESET_NONE;
    if (!bgfx::init(bgfxInit))
        ICESDK_CORE_CRITICAL("Failed to initialize BGFX!");

    ICESDK_CORE_INFO("BGFX Initialized...");

    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x000000FF, 1.0f,
                       0);

    bgfx::setViewRect(0, 0, 0, bgfx::BackbufferRatio::Equal);

#ifdef ICESDK_ANDROID
    // Set display size
    SDL_DisplayMode displayMode;
    SDL_GetDesktopDisplayMode(0, &displayMode);

    this->_width = static_cast<uint32_t>(displayMode.w);
    this->_height = static_cast<uint32_t>(displayMode.h);

    bgfx::reset(this->_width, this->_height, BGFX_RESET_NONE);
    bgfx::setViewRect(0, 0, 0, bgfx::BackbufferRatio::Equal);
#endif
}

GameWindow::~GameWindow()
{
    ICESDK_PROFILE_FUNCTION();
    bgfx::shutdown();

#ifdef ICESDK_GLFW
    glfwDestroyWindow(this->_window);
    glfwTerminate();
#endif
}

void GameWindow::Update()
{
    ICESDK_PROFILE_FUNCTION();

#ifdef ICESDK_GLFW
    glfwPollEvents();

    this->_should_exit = glfwWindowShouldClose(this->_window);
#endif

#ifdef ICESDK_SDL2
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0)
    {  // TODO: handle events
        if (e.type == SDL_QUIT) { _should_exit = true; }
        else if (e.type == SDL_WINDOWEVENT)
        {
            if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
            {
                ICESDK_CORE_INFO("Window size changed...");

                SDL_DisplayMode displayMode;
                SDL_GetDesktopDisplayMode(0, &displayMode);

                this->_width = static_cast<uint32_t>(displayMode.w);
                this->_height = static_cast<uint32_t>(displayMode.h);

                bgfx::reset(this->_width, this->_height, BGFX_RESET_NONE);
                bgfx::setViewRect(0, 0, 0, bgfx::BackbufferRatio::Equal);
            }
        }

        GetGameBase()->GetInputPipeline()->PumpSDL2Event(e);
    }
#endif

    const auto now = bx::getHPCounter();
    const auto frameTime = now - _last_time;
    _last_time = now;

    const auto freq = static_cast<float>(bx::getHPFrequency());
    const auto delta = static_cast<float>(frameTime) / freq;

    // This dummy draw call is here to make sure that view 0 is cleared if no
    // other draw calls are submitted to view 0.
    bgfx::touch(0);

    GetGameBase()->GetSpriteBatch()->NewFrame();
    if (this->_draw_init_callback != nullptr && !this->_initialized)
    {
        this->_draw_init_callback();
        this->_initialized = true;
    }
    if (this->_draw_callback != nullptr) this->_draw_callback(delta);
    // Advance to next frame. Main thread will be kicked to process submitted
    // rendering primitives.
    GetGameBase()->GetSpriteBatch()->EndFrame();
    bgfx::frame();
}

void GameWindow::SetDrawCallback(const DrawCallback_t pCallback)
{
    ICESDK_PROFILE_FUNCTION();
    this->_draw_callback = pCallback;
}

void GameWindow::SetDrawInitCallback(const DrawInitCallback_t pCallback)
{
    ICESDK_PROFILE_FUNCTION();
    this->_draw_init_callback = pCallback;
}

bool GameWindow::ShouldClose() const
{
    ICESDK_PROFILE_FUNCTION();

#if defined(ICESDK_GLFW) || defined(ICESDK_SDL2)
    return _should_exit;
#else
    #warning "Unknown Window Library"
    return false;
#endif
}

uint32_t GameWindow::Width() const
{
    ICESDK_PROFILE_FUNCTION();
    return this->_width;
}

uint32_t GameWindow::Height() const
{
    ICESDK_PROFILE_FUNCTION();
    return this->_height;
}
