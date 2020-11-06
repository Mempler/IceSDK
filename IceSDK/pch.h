#pragma once

#include "platform.h"

/* Headers */

#ifdef ICESDK_WIN32
    #define NOMINMAX
    #include <Windows.h>
#endif

#ifdef ICESDK_EMSCRIPTEN
    #include <emscripten.h>
#endif

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bimg/bimg.h>
#include <bimg/decode.h>
#include <bx/allocator.h>
#include <bx/bx.h>
#include <bx/timer.h>
#include <entt/entt.hpp>

#ifdef ICESDK_FMOD
    #include <fmod.h>
    #include <fmod.hpp>
    #include <fmod_errors.h>
#endif

#ifdef ICESDK_SDL2
    #include <SDL.h>
    #include <SDL_syswm.h>
#endif

// This is stupid... don't name thins "None" as a #define
#ifdef ICESDK_LINUX
    #undef None
#endif

#if ICESDK_ANDROID
    #include <GLES/egl.h>
#endif

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifdef ICESDK_SDL2
    #include <imgui_impl_sdl.h>
#endif

#include <spdlog/sinks/android_sink.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <ft2build.h>
#include <imgui.h>
#include <imgui_internal.h>

#include FT_FREETYPE_H

#include <hb-ft.h>
#include <hb.hh>

#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
