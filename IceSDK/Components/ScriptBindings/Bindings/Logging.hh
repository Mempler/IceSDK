#pragma once

#include "Scripting/Core.hh"

#include "Core/Utils/Logger.hh"

// INTERNAL FUNCTIONS! DO NOT CALL THOSE
namespace IceSDK::Script::Bindings::Natives
{
#define DECL_PRINT(TYPE)                                                       \
    uintptr_t Print_##TYPE(uintptr_t upArgs[NATIVE_STACK_SIZE])                \
    {                                                                          \
        const char* s0 = (const char*) upArgs[0];                              \
        const char* s1 = (const char*) upArgs[1];                              \
        const char* s2 = (const char*) upArgs[2];                              \
        const char* s3 = (const char*) upArgs[3];                              \
        const char* s4 = (const char*) upArgs[4];                              \
        const char* s5 = (const char*) upArgs[5];                              \
        const char* s6 = (const char*) upArgs[6];                              \
        const char* s7 = (const char*) upArgs[7];                              \
                                                                               \
        ICESDK_CORE_##TYPE(s0, s1, s2, s3, s4, s5, s6, s7);                    \
                                                                               \
        return 0;                                                              \
    }  // namespace Natives

    DECL_PRINT(TRACE)
    DECL_PRINT(INFO)
    DECL_PRINT(WARN)
    DECL_PRINT(ERROR)
    DECL_PRINT(CRITICAL)
}  // namespace IceSDK::Script::Bindings::Natives
