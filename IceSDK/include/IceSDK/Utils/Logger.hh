#pragma once

// https://github.com/TheCherno/Hazel/blob/master/Hazel/src/Hazel/Core/Log.h
// Apache2 license

#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/android_sink.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <memory>

namespace IceSDK::Utils
{
    class Log
    {
    private:
        inline static std::shared_ptr<spdlog::logger> s_pCoreLogger = nullptr;
        inline static std::shared_ptr<spdlog::logger> s_pClientLogger = nullptr;
        inline static bool g_LoggerInitialized = false;

    public:
        static void Init()
        {
            if (g_LoggerInitialized) return;

            std::vector<spdlog::sink_ptr> logSinks;
#ifdef ICESDK_ANDROID
            logSinks.emplace_back(
                std::make_shared<spdlog::sinks::android_sink_mt>());
#endif

#ifndef ICESDK_ANDROID
            logSinks.emplace_back(
                std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
            logSinks.emplace_back(
                std::make_shared<spdlog::sinks::basic_file_sink_mt>(
                    "IceSDK.log", true));
            logSinks[1]->set_pattern("[%T] [%l] %n: %v");
#endif

            logSinks[0]->set_pattern("%t\t[%T] [%n]\t[%5^%l%$]: %v");

            s_pCoreLogger = std::make_shared<spdlog::logger>(
                "ICESDK", begin(logSinks), end(logSinks));
            spdlog::register_logger(s_pCoreLogger);
            s_pCoreLogger->set_level(spdlog::level::trace);
            s_pCoreLogger->flush_on(spdlog::level::trace);

            s_pClientLogger = std::make_shared<spdlog::logger>(
                "APP", begin(logSinks), end(logSinks));
            spdlog::register_logger(s_pClientLogger);
            s_pClientLogger->set_level(spdlog::level::trace);
            s_pClientLogger->flush_on(spdlog::level::trace);

            g_LoggerInitialized = true;
        }

        static std::shared_ptr<spdlog::logger>& GetCoreLogger()
        {
            return s_pCoreLogger;
        }
        static std::shared_ptr<spdlog::logger>& GetClientLogger()
        {
            return s_pClientLogger;
        }
    };

}  // namespace IceSDK::Utils

// Core log macros
#define ICESDK_CORE_TRACE(...)                                                 \
    ::IceSDK::Utils::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define ICESDK_CORE_INFO(...)                                                  \
    ::IceSDK::Utils::Log::GetCoreLogger()->info(__VA_ARGS__)
#define ICESDK_CORE_WARN(...)                                                  \
    ::IceSDK::Utils::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define ICESDK_CORE_ERROR(...)                                                 \
    ::IceSDK::Utils::Log::GetCoreLogger()->error(__VA_ARGS__)
#define ICESDK_CORE_CRITICAL(...)                                              \
    ::IceSDK::Utils::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define ICESDK_TRACE(...)                                                      \
    ::IceSDK::Utils::Log::GetClientLogger()->trace(__VA_ARGS__)
#define ICESDK_INFO(...)                                                       \
    ::IceSDK::Utils::Log::GetClientLogger()->info(__VA_ARGS__)
#define ICESDK_WARN(...)                                                       \
    ::IceSDK::Utils::Log::GetClientLogger()->warn(__VA_ARGS__)
#define ICESDK_ERROR(...)                                                      \
    ::IceSDK::Utils::Log::GetClientLogger()->error(__VA_ARGS__)
#define ICESDK_CRITICAL(...)                                                   \
    ::IceSDK::Utils::Log::GetClientLogger()->critical(__VA_ARGS__)
