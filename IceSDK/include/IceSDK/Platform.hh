#pragma once

#ifdef __linux__
    #define ICESDK_LINUX 1
#endif

#ifdef _WIN32
    #define ICESDK_WIN32 1
#endif

#ifdef __EMSCRIPTEN__
    #define ICESDK_EMSCRIPTEN 1
#endif

#ifdef __ANDROID__
    #define ICESDK_ANDROID 1
#endif
