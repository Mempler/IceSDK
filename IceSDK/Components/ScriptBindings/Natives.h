#ifndef ICESDK_SCRIPTING_NATIVES
#define ICESDK_SCRIPTING_NATIVES

#include "Scripting/Native.h"

#define countof(a) (sizeof(a) / sizeof((a)[0]))

// Wrapper around Engine Functions

#define _NATIVE_PRINT(LOGGER, ...)                                             \
    uintptr_t native = Native_Init(Native_Hash(LOGGER));                       \
    const void* args[] = { __VA_ARGS__ };                                      \
                                                                               \
    for (size_t i = 0; i < countof(args); i++)                                 \
        Native_PushArgument(native, (uintptr_t) args[i]);                      \
                                                                               \
    Native_Execute(native)

#define Native_PRINT(...) _NATIVE_PRINT("CORE_PRINT", __VA_ARGS__)
#define Native_PRINT_TRACE(...) _NATIVE_PRINT("CORE_PRINT_TRACE", __VA_ARGS__)
#define Native_PRINT_INFO(...) _NATIVE_PRINT("CORE_PRINT_INFO", __VA_ARGS__)
#define Native_PRINT_WARN(...) _NATIVE_PRINT("CORE_PRINT_WARN", __VA_ARGS__)
#define Native_PRINT_ERR(...) _NATIVE_PRINT("CORE_PRINT_ERR", __VA_ARGS__)
#define Native_PRINT_CRIT(...) _NATIVE_PRINT("CORE_PRINT_CRIT", __VA_ARGS__)

#endif
