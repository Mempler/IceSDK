#pragma once

#include "IceSDK/Platform.hh"
#include "IceSDK/Utils/Exception.hh"

#include <unistd.h>

#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <string_view>
#include <vector>

#ifdef ICESDK_WIN32
    #include <Windows.h>
#elif defined(ICESDK_LINUX) || defined(ICESDK_ANDROID)
    #include <sys/stat.h>

    #include <dirent.h>
    #include <fcntl.h>

    #include <cstdlib>
#elif defined(ICESDK_EMSCRIPTEN)
    #include <sys/stat.h>

    #include <dirent.h>
    #include <emscripten.h>
    #include <stdlib.h>
#else
    #error "Platform not implemented!"
#endif

#define FS_EXCEPTION(_FMT, ...)                                                \
    ::IceSDK::Utils::FileSystem::Exception(fmt::format(_FMT, __VA_ARGS__),     \
                                           __FILE__, __LINE__)

namespace IceSDK::Utils::FileSystem
{
    class Exception final : public BaseException
    {
    public:
        explicit Exception(std::string_view svReason, const char* svWhere,
                           size_t sLine) :
            BaseException("FileSystem", svReason, svWhere, sLine)
        {
        }
    };

    /*****************************************************
     * GetFileName
     *
     * Gets the file name from a given path
     *
     * @param svPath Path of a file
     *
     * @return Resulting file name
     *****************************************************/
    constexpr std::string_view GetFileName(std::string_view svPath) noexcept
    {
        for (auto i = svPath.size(); i > 0; --i)
            if (svPath[i] == '/' || svPath[i] == '\\')
                return std::string_view(&svPath[0] + (i + 1));

        return svPath;  // Not found...
    }

    /*****************************************************
     * HasExtension
     *
     * Checks if a file has an specific extension
     *
     * @param svPath Path of a file
     * @param svExt Extension which should be looked for
     *
     * @return true if has the given extension
     *****************************************************/
    constexpr bool HasExtension(std::string_view svPath,
                                std::string_view svExt) noexcept
    {
        if (svPath.length() < svExt.length()) return false;

        // Check if the last characters match svExt
        return (0
                == svPath.compare(svPath.length() - svExt.length(),
                                  svExt.length(), svExt));
    }

    /*****************************************************
     * CreateDirectory
     *
     * Creates a directory the simple way
     * It'll fail if the given directory exists
     * or if no permissions to access the parent
     * directory
     *
     * @param svPath Path for the new Directory
     *
     * @return false if failed
     *****************************************************/
    inline bool CreateDirectory(std::string_view svPath)
    {
#ifdef ICESDK_WIN32
        return CreateDirectory(svPath.data(), NULL);
#elif defined(ICESDK_LINUX) || defined(ICESDK_EMSCRIPTEN)                      \
    || defined(ICESDK_ANDROID)
        if (mkdir(svPath.data(), umask(0755)))
        {
            chmod(svPath.data(),
                  umask(0755));  // set the correct permissions
                                 // cause it's wrong for some reason
            return true;
        }
#else
    #error "Platform not implemented!"
#endif
        return false;
    }

    /*****************************************************
     * JoinPath
     *
     * Joins 2 paths together into one
     * E.G "some/path" + "other" = "some/other/path"
     *
     * @param svPath1 Input path 1
     * @param svPath2 Input path 2
     *
     * @return Joined file path
     *****************************************************/
    inline std::string JoinPath(std::string_view svPath1,
                                std::string_view svPath2)
    {
        // Make sure our paths are valid
        if (svPath1.length() <= 0) return svPath2.data();
        if (svPath2.length() <= 0) return svPath1.data();

        if (svPath1[svPath1.length() - 1] == '/'
            || svPath1[svPath1.length() - 1] == '\\')
            return std::string(svPath1.data()) + svPath2.data();

        return std::string(svPath1) + "/" + svPath2.data();
    }

    /*****************************************************
     * Exists
     *
     * Checks if a Path exists
     *
     * @param svPath Input path
     *
     * @return true if exists
     *****************************************************/
    inline bool Exists(std::string_view svPath)
    {
#ifdef ICESDK_WIN32
        const auto dir_type = GetFileAttributesA(svPath.data());

        return dir_type != INVALID_FILE_ATTRIBUTES;
#elif defined(ICESDK_LINUX) || defined(ICESDK_EMSCRIPTEN)                      \
    || defined(ICESDK_ANDROID)
        return (access(svPath.data(), F_OK) != -1);
#else
    #error "Platform not implemented!"
#endif
        return false;
    }

    /*****************************************************
     * IsDirectory
     *
     * Checks if a Path is a Directory
     *
     * @param svPath Input path
     *
     * @return true if is a directory
     *****************************************************/
    inline bool IsDirectory(std::string_view svPath)
    {
        if (!Exists(svPath)) return false;

#ifdef ICESDK_WIN32
        return GetFileAttributes(svPath.data()) & FILE_ATTRIBUTE_DIRECTORY;
#elif defined(ICESDK_LINUX) || defined(ICESDK_EMSCRIPTEN)                      \
    || defined(ICESDK_ANDROID)
        struct stat st
        {
        };
        stat(svPath.data(), &st);
        return S_ISDIR(st.st_mode);
#else
    #error "Platform not implemented!"
#endif
    }

    /*****************************************************
     * ReadDirectory
     *
     * Reads a directory and returns it's content paths
     *
     * @param svPath Input path
     * @param bRecursive Recursive directory reading
     *
     * @return All paths in given path
     *****************************************************/
    inline std::vector<std::string> ReadDirectory(std::string_view svPath,
                                                  bool bRecursive = false)
    {
        std::vector<std::string> directories;

#if defined(ICESDK_WIN32)
        HANDLE hFind;
        WIN32_FIND_DATA FindFileData;

        // Requires a * wildcard for some reason
        if ((hFind = FindFirstFile(FileSystem::JoinPath(svPath, "./*").data(),
                                   &FindFileData))
            != INVALID_HANDLE_VALUE)
        {
            do {
                if (std::string_view(FindFileData.cFileName) == "."
                    || std::string_view(FindFileData.cFileName) == "..")
                    continue;

                directories.push_back(
                    FileSystem::JoinPath(svPath, FindFileData.cFileName));
            } while (FindNextFile(hFind, &FindFileData));
            FindClose(hFind);
        }
#elif defined(ICESDK_LINUX) || defined(ICESDK_EMSCRIPTEN)                      \
    || defined(ICESDK_ANDROID)
        DIR* dir;
        struct dirent* ent;
        if ((dir = opendir(svPath.data())) != nullptr)
        {
            while ((ent = readdir(dir)) != nullptr)
            {
                if (std::string_view(ent->d_name) == "."
                    || std::string_view(ent->d_name) == "..")
                    continue;

                directories.push_back(
                    FileSystem::JoinPath(svPath, ent->d_name));
            }
            closedir(dir);
        }
#else
    #error "Platform not implemented!"
#endif

        if (bRecursive && !directories.empty())
        {
            for (const auto& _dir : directories)
            {
                if (FileSystem::IsDirectory(_dir))
                {
                    for (const auto& __dir : ReadDirectory(_dir, true))
                    {
                        directories.push_back(__dir);
                    }
                }
            }
        }

        return directories;
    }

    /*****************************************************
     * ResolveFullPath
     *
     * Resolves a full path of an directory
     *
     * @param svPath Input path
     *
     * @return full path
     *****************************************************/
    inline std::string ResolveFullPath(std::string_view svPath)
    {
#ifdef ICESDK_WIN32
        char buffer[4096];
        if (IS_ERROR(GetFullPathNameA(svPath.data(), 4096, buffer, nullptr)))
            throw FS_EXCEPTION("Failed to resolve full path of {}", svPath);

        return buffer;
#elif defined(ICESDK_LINUX) || defined(ICESDK_EMSCRIPTEN)                      \
    || defined(ICESDK_ANDROID)
        char buffer[PATH_MAX + 1];

        if (realpath(svPath.data(), buffer) == nullptr)
            throw FS_EXCEPTION("Failed to resolve full path of {}", svPath);

        return std::string(buffer);
#else
    #error "Platform not implemented!"
#endif
    }

    /*****************************************************
     * ReadBinaryFile
     *
     * Reads a binary file and returns it's content
     *
     * @param svPath Input path
     *
     * @return content
     *****************************************************/
    inline std::vector<uint8_t> ReadBinaryFile(std::string_view svPath)
    {
        std::ifstream file(svPath.data(), std::ios::binary | std::ios::in);
        if (!file && file.fail())
            throw FS_EXCEPTION("Failed to open {}", svPath);

        file.unsetf(std::ios::skipws);  // Read the file properly!

        file.seekg(0, std::ios::end);
        const auto fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        // If we got nothing, just return an empty vector.
        if (fileSize <= 0) return {};

        std::vector<uint8_t> data;
        data.reserve(fileSize);

        data.insert(data.begin(), std::istream_iterator<uint8_t>(file),
                    std::istream_iterator<uint8_t>());

        return data;
    }

    /*****************************************************
     * WriteBinaryFile
     *
     * Writes a binary file
     *
     * @param svPath Input path
     * @param vContent Content
     *****************************************************/
    inline void WriteBinaryFile(std::string_view svPath,
                                const std::vector<uint8_t>& vContent)
    {
#if defined(ICESDK_LINUX) || defined(ICESDK_ANDROID)
        auto fp = open(svPath.data(), O_WRONLY | O_CREAT, umask(0755));
        if (fp < 0) throw FS_EXCEPTION("Failed to write {}", svPath);

        // TODO: if (fp < 0) ICESDK_CORE_ERROR("Failed to open {}",
        // svPath);

        write(fp, svPath.data(), vContent.size());

        close(fp);
#else
        std::ofstream file(svPath.data(), std::ios::binary | std::ios::out);
        if (!file && file.fail())
            throw FS_EXCEPTION("Failed to open {}", svPath);

        file.unsetf(std::ios::skipws);

        file.write((const char*) vContent.data(), vContent.size());
        file.close();
#endif

#if defined(ICESDK_LINUX) || defined(ICESDK_EMSCRIPTEN)                        \
    || defined(ICESDK_ANDROID)
        chmod(svPath.data(),
              umask(0755));  // set the correct permissions cause it's wrong
#endif
    }

    /*****************************************************
     * Touch
     *
     * Creates an empty file
     *
     * @param svPath Input path
     *****************************************************/
    inline void Touch(std::string_view svPath) { WriteBinaryFile(svPath, {}); }

    /*****************************************************
     * Delete
     *
     * Deletes a file
     *
     * @param svPath Input path
     *****************************************************/
    inline void Delete(std::string_view svPath) { remove(svPath.data()); }
}  // namespace IceSDK::Utils::FileSystem
