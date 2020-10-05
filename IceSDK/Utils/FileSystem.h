#pragma once

#include <string>
#include <vector>

namespace IceSDK
{
    namespace FileSystem
    {
        constexpr std::string_view GetFileName(std::string_view pPath) noexcept
        {
            for (auto i = pPath.size(); i > 0; --i)
                if (pPath[i] == '/' || pPath[i] == '\\')
                    return std::string_view(&pPath[0] + (i + 1));

            return pPath;  // Not found...
        }
        constexpr bool HasExtension(std::string_view pPath,
                                    std::string_view pExt) noexcept
        {
            if (pPath.length() < pExt.length()) return false;

            return (0
                    == pPath.compare(pPath.length() - pExt.length(),
                                     pExt.length(), pExt));
        }

        void MkDir(std::string_view pPath);
        std::string JoinPath(std::string_view p1, std::string_view p2);
        bool Exists(std::string_view pPath);
        bool IsDirectory(std::string_view pPath);
        std::vector<std::string> ReadDirectory(std::string_view pPath,
                                               bool pRecursive = false);
        std::string ResolveFullPath(std::string_view pPath);
        std::vector<uint8_t> ReadBinaryFile(std::string_view pPath);
        void WriteBinaryFile(std::string_view pPath,
                             const std::vector<uint8_t>& pData);
        void Touch(std::string_view pPath);
        void Delete(std::string_view pPath);
    }  // namespace FileSystem
}  // namespace IceSDK
