#ifndef ICESDK_PLATFORM_H
#define ICESDK_PLATFORM_H

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

#if ICESDK_WIN32
    #ifdef ICESDK_BUILD_SHARED
        #define IceSDK_EXPORT __declspec(dllexport)
    #else
        #define IceSDK_EXPORT __declspec(dllimport)
    #endif
#endif

#if ICESDK_LINUX
    #ifdef ICESDK_BUILD_SHARED
        #define IceSDK_EXPORT __attribute__((visibility("default")))
    #else
        #define IceSDK_EXPORT
    #endif
#endif

#endif