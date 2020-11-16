#ifndef ICESDK_SCRIPTING_ENGINECALLS
#define ICESDK_SCRIPTING_ENGINECALLS

#include <stdint.h>

#define NATIVE_STACK_SIZE 8

#define NATIVE_SUCCESS 0x01
#define NATIVE_ERROR_STACK_FULL -0x01
#define NATIVE_ERROR_INVALID_ARG -0x02

#ifdef __cplusplus
    #define NATIVE_EXTERN extern "C"
    #define CONSTEXPR constexpr
#else
    #define NATIVE_EXTERN
    #define CONSTEXPR inline const
#endif

/*****************************************************
 * Native_Hash
 *
 * Calculates a function name hash
 *
 * @param cStr function name
 *
 * @return 0x00 = Failed
 *****************************************************/
#include <stdio.h>

CONSTEXPR uint64_t Native_Hash(const char* cStr)
{
    if (!cStr) return 0x00;

    uint64_t hash = 0x00;

    while (*cStr != 0x00)
    {
        hash += *cStr;
        hash += (hash << 10);
        hash ^= (hash >> 6);

        cStr++;
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}

/*****************************************************
 * Native_Init
 *
 * Initializes an engine call
 *
 * @param uHash function name hash
 *
 * @return Might be NULL if failed
 *****************************************************/
NATIVE_EXTERN uintptr_t Native_Init(uint64_t uHash);

/*****************************************************
 * Native_PushArgument
 *
 * Pushes an argument to the engine call stack
 *
 * @param upHandler native handler
 * @param upArgument Argument pointer
 *                   (wont be free'd after execute!)
 *
 * @return < 0 Error code
 *         > 0 Success
 *****************************************************/
NATIVE_EXTERN int Native_PushArgument(uintptr_t upHandler,
                                      uintptr_t upArgument);

/*****************************************************
 * Native_Execute
 *
 * Executes an Engine Call
 *
 * @param upHandler native handler
 *
 * @return Might be NULL if failed
 *****************************************************/
NATIVE_EXTERN uintptr_t Native_Execute(uintptr_t upHandler);

#endif
