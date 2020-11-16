#include "Lib.hh"

#include "Scripting/Core.hh"

#include "Bindings/Logging.hh"

void IceSDK::Script::Bindings::Init()
{
    // Register logging target
    g_pScriptingCore->RegisterNative("CORE_PRINT"_h, Natives::Print_INFO);
    g_pScriptingCore->RegisterNative("CORE_PRINT_TRACE"_h,
                                     Natives::Print_TRACE);
    g_pScriptingCore->RegisterNative("CORE_PRINT_INFO"_h, Natives::Print_INFO);
    g_pScriptingCore->RegisterNative("CORE_PRINT_WARN"_h, Natives::Print_WARN);
    g_pScriptingCore->RegisterNative("CORE_PRINT_ERR"_h, Natives::Print_ERROR);
    g_pScriptingCore->RegisterNative("CORE_PRINT_CRIT"_h,
                                     Natives::Print_CRITICAL);
}
