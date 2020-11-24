#pragma once

#include <spdlog/fmt/bundled/core.h>

#include <exception>
#include <string_view>

namespace IceSDK::Utils
{
    class BaseException : public std::exception
    {
    private:
        std::string m_sFormated;

    protected:
        explicit BaseException(const char* csWho, fmt::string_view csReason,
                               const char* csWhere, size_t sLine) noexcept
        {
            m_sFormated = fmt::format("{}: {}\n    At: {}:{}", csWho, csReason,
                                      csWhere, sLine);
        }

    public:
        const char* what() const noexcept { return m_sFormated.c_str(); }
    };
}  // namespace IceSDK::Utils
