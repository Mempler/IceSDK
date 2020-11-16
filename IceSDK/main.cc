#include "ScriptBindings/Lib.hh"
#include "ScriptBindings/Natives.h"
#include "Scripting/Core.hh"

#include "Core/Graphics/Window.hh"
#include "Core/Utils/Logger.hh"

#include <iostream>
#include <thread>

static IceSDK::Core::Graphics::GameWindow* g_GameWindow = nullptr;
static bool g_bDrawThreadExit = false;

/*****************************************************
 * DrawThread
 *
 * Draw thread function for drawing stuff...
 * who would've guessed?
 *
 * @return Error code
 *         0 = Success
 *****************************************************/
int DrawThread()
{
    ICESDK_CORE_TRACE("Intialized Draw Thread");

    g_GameWindow = new IceSDK::Core::Graphics::GameWindow;

    while (true)
    {
        // Tick required global variables
        g_GameWindow->Tick();

        g_pScriptingCore->Tick(IceSDK::Script::ThreadType::Render);

        if (g_GameWindow->ShouldExit()) break;
    }

    ICESDK_CORE_TRACE("Draw thread exit...");
    g_bDrawThreadExit = true;

    return 0;
}

/*****************************************************
 * TickThread
 *
 * Tick thread function for updating stuff...
 * who would've guessed?
 *
 * @return Error code
 *         0 = Success
 *****************************************************/
int TickThread()
{
    ICESDK_CORE_TRACE("Intialized Tick Thread");

    while (true)
    {
        g_pScriptingCore->Tick(IceSDK::Script::ThreadType::Tick);

        if (g_bDrawThreadExit) break;
    }

    ICESDK_CORE_TRACE("Tick thread exit...");

    return 0;
}

int main()
{
    IceSDK::Core::Utils::Log::Init();
    IceSDK::Script::ScriptingCore::Init();
    IceSDK::Script::Bindings::Init();

    std::thread tDraw(DrawThread);
    std::thread tTick(TickThread);

    tDraw.join();

    // Draw thread exited, notify our Tick Thread
    // TODO: Command pipeline

    tTick.join();

    return 0;
}
