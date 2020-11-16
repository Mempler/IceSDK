#include "pch.hh"

#include "Utils/Logger.hh"

// https://github.com/TheCherno/Hazel/blob/master/Hazel/src/Hazel/Core/Log.h
// Apache2 license

using namespace IceSDK::Core::Utils;

std::shared_ptr<spdlog::logger> Log::s_pCoreLogger;
std::shared_ptr<spdlog::logger> Log::s_pClientLogger;

static bool g_LoggerInitialized = false;

void Log::Init()
{
    if (g_LoggerInitialized) return;

    std::vector<spdlog::sink_ptr> logSinks;
#ifdef ICESDK_ANDROID
    logSinks.emplace_back(std::make_shared<spdlog::sinks::android_sink_mt>());
#endif

#ifndef ICESDK_ANDROID
    logSinks.emplace_back(
        std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
    logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(
        "IceSDK.log", true));
    logSinks[1]->set_pattern("[%T] [%l] %n: %v");
#endif

    logSinks[0]->set_pattern("%t\t[%T] [%n]\t[%5^%l%$]: %v");

    s_pCoreLogger = std::make_shared<spdlog::logger>("ICESDK", begin(logSinks),
                                                     end(logSinks));
    spdlog::register_logger(s_pCoreLogger);
    s_pCoreLogger->set_level(spdlog::level::trace);
    s_pCoreLogger->flush_on(spdlog::level::trace);

    s_pClientLogger =
        std::make_shared<spdlog::logger>("APP", begin(logSinks), end(logSinks));
    spdlog::register_logger(s_pClientLogger);
    s_pClientLogger->set_level(spdlog::level::trace);
    s_pClientLogger->flush_on(spdlog::level::trace);

    g_LoggerInitialized = true;
}
