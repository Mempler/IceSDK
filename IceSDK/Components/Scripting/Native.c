#include "pch.h"

#include "Native.h"

#pragma pack(push)
typedef struct
{
    uint64_t m_uHash;
    uint8_t m_uStackPos;
    uintptr_t m_upStack[NATIVE_STACK_SIZE];
} native_t;
#pragma pack(pop)

/*****************************************************
 * Native_Init
 *
 * Initializes an engine call
 *
 * @param uHash function name hash
 *
 * @return Might be NULL if failed
 *****************************************************/
uintptr_t Native_Init(uint64_t uHash)
{
    if (uHash == 0) return 0;

    native_t* native = (native_t*) calloc(1, sizeof(native_t));

    native->m_uHash = uHash;

    return (uintptr_t) native;
}

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
int Native_PushArgument(uintptr_t upHandler, uintptr_t upArgument)
{
    if (upHandler == 0 || upArgument == 0) return NATIVE_ERROR_INVALID_ARG;

    native_t* native = (native_t*) upHandler;

    // If stack is full, we just return an error
    if (native->m_uStackPos == NATIVE_STACK_SIZE)
        return NATIVE_ERROR_STACK_FULL;

    native->m_upStack[native->m_uStackPos] = upArgument;
    native->m_uStackPos += 1;

    return 1;
}

typedef uintptr_t (*NativeEntry)(uintptr_t upArgs[NATIVE_STACK_SIZE]);
extern NativeEntry _get_native(uint64_t uHash);

/*****************************************************
 * Native_Execute
 *
 * Executes an Engine Call
 *
 * @param upHandler native handler
 *
 * @return Might be NULL if failed
 *****************************************************/
uintptr_t Native_Execute(uintptr_t upHandler)
{
    if (upHandler == 0) return 0;

    native_t* native = (native_t*) upHandler;

    NativeEntry nativeEntry = _get_native(native->m_uHash);
    if (nativeEntry == NULL) return 0;

    (*nativeEntry)(native->m_upStack);

    free(native);
}
