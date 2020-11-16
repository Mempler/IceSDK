#pragma once

// https://github.com/TheCherno/Hazel/blob/master/Hazel/src/Hazel/Core/Log.h
// Apache2 license

#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>

namespace IceSDK::Core::Utils
{
    class Log
    {
    private:
        static std::shared_ptr<spdlog::logger> s_pCoreLogger;
        static std::shared_ptr<spdlog::logger> s_pClientLogger;

    public:
        static void Init();

        static std::shared_ptr<spdlog::logger>& GetCoreLogger()
        {
            return s_pCoreLogger;
        }
        static std::shared_ptr<spdlog::logger>& GetClientLogger()
        {
            return s_pClientLogger;
        }
    };

}  // namespace IceSDK::Core::Utils

// Core log macros
#define ICESDK_CORE_TRACE(...)                                                 \
    ::IceSDK::Core::Utils::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define ICESDK_CORE_INFO(...)                                                  \
    ::IceSDK::Core::Utils::Log::GetCoreLogger()->info(__VA_ARGS__)
#define ICESDK_CORE_WARN(...)                                                  \
    ::IceSDK::Core::Utils::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define ICESDK_CORE_ERROR(...)                                                 \
    ::IceSDK::Core::Utils::Log::GetCoreLogger()->error(__VA_ARGS__)
#define ICESDK_CORE_CRITICAL(...)                                              \
    ::IceSDK::Core::Utils::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define ICESDK_TRACE(...)                                                      \
    ::IceSDK::Core::Utils::Log::GetClientLogger()->trace(__VA_ARGS__)
#define ICESDK_INFO(...)                                                       \
    ::IceSDK::Core::Utils::Log::GetClientLogger()->info(__VA_ARGS__)
#define ICESDK_WARN(...)                                                       \
    ::IceSDK::Core::Utils::Log::GetClientLogger()->warn(__VA_ARGS__)
#define ICESDK_ERROR(...)                                                      \
    ::IceSDK::Core::Utils::Log::GetClientLogger()->error(__VA_ARGS__)
#define ICESDK_CRITICAL(...)                                                   \
    ::IceSDK::Core::Utils::Log::GetClientLogger()->critical(__VA_ARGS__)
