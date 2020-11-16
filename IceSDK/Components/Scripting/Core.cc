#include "Core.hh"

#include <iostream>

using namespace IceSDK::Script;

ScriptingCore* g_pScriptingCore = nullptr;

/*****************************************************
 * Init
 *
 * Initializes ScriptingCore
 *
 *****************************************************/
void ScriptingCore::Init()
{
    g_pScriptingCore = new ScriptingCore;
}

/*****************************************************
 * GetNative
 *
 * Gets a Native Function ready to be called
 *
 * @param uHash Native Name Hash
 *
 * @return NULL if failed
 *****************************************************/
NativeEntry ScriptingCore::GetNative(uint64_t uHash)
{
    return this->m_umNatives[uHash];
}

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
 * @param fEntry Script entry
 * @param eThreadType Which thread shall this script
 *                    run on?
 *****************************************************/
void ScriptingCore::RegisterScript(ScriptEntry fEntry, ThreadType eThreadType)
{
    fEntry();  // For now lets run it on the main thread
}

/*****************************************************
 * RegisterNative
 *
 * Registers an Native later to be run inside Scripts
 *
 * @param uHash Native Name Hash
 * @param fEntry Native entry
 *
 * @return false if failed
 *****************************************************/
void ScriptingCore::RegisterNative(uint64_t uHash, NativeEntry fEntry)
{
    if (this->m_umNatives.find(uHash) != this->m_umNatives.end())
    {
        std::cout << "Failed to register native: Another native with the same "
                     "Hash exists"
                  << std::endl;
        return;
    }

    this->m_umNatives.insert(std::make_pair(uHash, fEntry));
}

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
void ScriptingCore::Tick(ThreadType eThreadType)
{
    for (auto&& entry : this->m_umvThreadEntries[eThreadType]) { entry(); }
}

// Internal function for our C core
extern "C" NativeEntry _get_native(uint64_t uHash)
{
    return g_pScriptingCore->GetNative(uHash);
}