#pragma once

#include <unordered_map>

#include "Native.h"

#include <cstdint>
#include <vector>

namespace IceSDK::Script
{
    enum ThreadType
    {
        Tick,   // This will always run on the Tick thread
        Render  // This will always run on the Render thread
    };

    typedef uintptr_t (*NativeEntry)(uintptr_t upArgs[NATIVE_STACK_SIZE]);
    typedef int (*ScriptEntry)();

    class ScriptingCore
    {
    private:
        std::unordered_map<uint64_t, NativeEntry> m_umNatives;
        std::unordered_map<ThreadType, std::vector<ScriptEntry>>
            m_umvThreadEntries;

    public:
        /*****************************************************
         * Init
         *
         * Initializes ScriptingCore
         *
         *****************************************************/
        static void Init();

        /*****************************************************
         * GetNative
         *
         * Gets a Native Function ready to be called
         *
         * @param uHash Native Name Hash
         *
         * @return NULL if failed
         *****************************************************/
        NativeEntry GetNative(uint64_t uHash);

        /*****************************************************
         * RegisterScript
         *
         * Registers a script to later be run on any declared
         * Thread
         *
         * NOTICE: This might run multiple times
         *         once per tick
         *         So make sure you have some
         *         Global variables ready
         *
         * NOTICE: This can only be called from MainThread!
         *
         * @param fEntry Script entry
         * @param eThreadType Which thread shall this script
         *                    run on?
         *****************************************************/
        void RegisterScript(ScriptEntry fEntry,
                            ThreadType eThreadType = ThreadType::Tick);

        /*****************************************************
         * RegisterNative
         *
         * Registers an Native later to be run inside Scripts
         *
         * NOTICE: This can only be called from MainThread!
         *
         * @param uHash Native Name Hash
         * @param fEntry Native entry
         *
         * @return false if failed
         *****************************************************/
        void RegisterNative(uint64_t uHash, NativeEntry fEntry);

        /*****************************************************
         * Tick
         *
         * Ticks the specified thread
         *
         * NOTICE: DO NOT RUN THIS FUNCTION OUTSIDE OF
         *         IT'S INTENDED THREAD!
         *
         * @param eThreadType Thread that shall be ticked
         *
         * @return false if failed
         *****************************************************/
        void Tick(ThreadType eThreadType = ThreadType::Tick);
    };
}  // namespace IceSDK::Script

extern IceSDK::Script::ScriptingCore* g_pScriptingCore;

constexpr uint64_t operator""_h(const char* cStr, long unsigned int)
{
    return Native_Hash(cStr);
}
