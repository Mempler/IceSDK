#include "pch.h"

#include "Utils/FileSystem.h"

#include "Utils/Logger.h"

#include <fstream>
#include <iostream>
#include <iterator>

#ifdef ICESDK_WIN32
    #include <Windows.h>
#elif defined(ICESDK_LINUX) || defined(ICESDK_ANDROID)
    #include <sys/stat.h>
    #include <sys/types.h>

    #include <dirent.h>
    #include <stdlib.h>
    #include <unistd.h>

#elif defined(ICESDK_EMSCRIPTEN)
    #include <sys/stat.h>

    #include <dirent.h>
    #include <emscripten.h>
    #include <stdlib.h>
    #include <unistd.h>

#else
    #error "Platform not implemented!"
#endif

using namespace IceSDK;

std::string FileSystem::JoinPath(std::string_view p1, std::string_view p2)
{
    if (p1.length() <= 0) return p2.data();

    if (p2.length() <= 0) return p1.data();

    if (p1[p1.length() - 1] == '/' || p1[p1.length() - 1] == '\\')
        return std::string(p1.data()) + p2.data();

    return std::string(p1) + "/" + p2.data();
}

bool FileSystem::Exists(std::string_view pPath)
{
#ifdef ICESDK_WIN32
    const auto dir_type = GetFileAttributesA(pPath.data());

    return dir_type != INVALID_FILE_ATTRIBUTES;
#elif defined(ICESDK_LINUX) || defined(ICESDK_EMSCRIPTEN)                      \
    || defined(ICESDK_ANDROID)
    return (access(pPath.data(), F_OK) != -1);
#else
    #error "Platform not implemented!"
#endif
    return false;
}

bool FileSystem::IsDirectory(std::string_view pPath)
{
    if (!Exists(pPath)) return false;

#ifdef ICESDK_WIN32
    return GetFileAttributes(pPath.data()) & FILE_ATTRIBUTE_DIRECTORY;
#elif defined(ICESDK_LINUX) || defined(ICESDK_EMSCRIPTEN)                      \
    || defined(ICESDK_ANDROID)
    struct stat st;
    stat(pPath.data(), &st);
    return S_ISDIR(st.st_mode);
#else
    #error "Platform not implemented!"
#endif
    return false;
}

std::vector<std::string> FileSystem::ReadDirectory(std::string_view pPath,
                                                   bool pRecursive /*= false */)
{
    std::vector<std::string> directories;

#if defined(ICESDK_WIN32)
    HANDLE hFind;
    WIN32_FIND_DATA FindFileData;

    // Requires a * wildcard for some reason
    if ((hFind = FindFirstFile(FileSystem::JoinPath(pPath, "./*").data(),
                               &FindFileData))
        != INVALID_HANDLE_VALUE)
    {
        do
        {
            if (std::string_view(FindFileData.cFileName) == "."
                || std::string_view(FindFileData.cFileName) == "..")
                continue;

            directories.push_back(
                FileSystem::JoinPath(pPath, FindFileData.cFileName));
        } while (FindNextFile(hFind, &FindFileData));
        FindClose(hFind);
    }
#elif defined(ICESDK_LINUX) || defined(ICESDK_EMSCRIPTEN)                      \
    || defined(ICESDK_ANDROID)
    DIR* dir;
    struct dirent* ent;
    if ((dir = opendir(pPath.data())) != nullptr)
    {
        while ((ent = readdir(dir)) != nullptr)
        {
            if (std::string_view(ent->d_name) == "."
                || std::string_view(ent->d_name) == "..")
                continue;

            directories.push_back(FileSystem::JoinPath(pPath, ent->d_name));
        }
        closedir(dir);
    }
#else
    #error "Platform not implemented!"
#endif

    if (pRecursive && !directories.empty())
    {
        for (const auto& _dir : directories)
        {
            if (FileSystem::IsDirectory(_dir))
            {
                for (const auto& __dir : ReadDirectory(_dir, true))
                { directories.push_back(__dir); }
            }
        }
    }

    return directories;
}

std::string FileSystem::ResolveFullPath(std::string_view pPath)
{
#ifdef ICESDK_WIN32
    char buffer[4096];
    if (IS_ERROR(GetFullPathNameA(pPath.data(), 4096, buffer, nullptr)))
        return "";

    return buffer;
#elif defined(ICESDK_LINUX) || defined(ICESDK_EMSCRIPTEN)                      \
    || defined(ICESDK_ANDROID)
    char buffer[PATH_MAX + 1];

    realpath(pPath.data(), buffer);

    return std::string(buffer);
#else
    #error "Platform not implemented!"
#endif
    return "";
}

std::vector<uint8_t> FileSystem::ReadBinaryFile(std::string_view pPath)
{
    std::ifstream file(pPath.data(), std::ios::binary | std::ios::in);
    if (!file && file.fail())
    {
        ICESDK_CORE_ERROR("Failed to open {}", pPath);
        return {};
    }

    file.unsetf(std::ios::skipws);  // Read the file properly!

    file.seekg(0, std::ios::end);
    const auto fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    if (fileSize <= 0) return {};

    std::vector<uint8_t> data;
    data.reserve(fileSize);

    data.insert(data.begin(), std::istream_iterator<uint8_t>(file),
                std::istream_iterator<uint8_t>());

    return data;
}

void FileSystem::WriteBinaryFile(std::string_view pPath,
                                 const std::vector<uint8_t>& pData)
{
#if defined(ICESDK_LINUX) || defined(ICESDK_ANDROID)
    auto fp = open(pPath.data(), O_WRONLY | O_CREAT, umask(0755));
    if (fp < 0) ICESDK_CORE_ERROR("Failed to open {}", pPath);

    write(fp, pData.data(), pData.size());

    close(fp);
#else
    std::ofstream file(pPath.data(), std::ios::binary | std::ios::out);
    if (!file && file.fail()) ICESDK_CORE_ERROR("Failed to open {}", pPath);

    file.unsetf(std::ios::skipws);

    file.write((const char*) pData.data(), pData.size());
    file.close();
#endif

#if defined(ICESDK_LINUX) || defined(ICESDK_EMSCRIPTEN)                        \
    || defined(ICESDK_ANDROID)
    chmod(pPath.data(),
          umask(0755));  // set the correct permissions cause it's wrong
#endif
}

void FileSystem::MkDir(std::string_view pPath)
{
#ifdef ICESDK_WIN32
    CreateDirectory(pPath.data(), NULL);
#elif defined(ICESDK_LINUX) || defined(ICESDK_EMSCRIPTEN)                      \
    || defined(ICESDK_ANDROID)
    if (mkdir(pPath.data(), umask(0755)))
        ICESDK_CORE_ERROR("Failed to create Directory! {}", pPath);

    chmod(pPath.data(), umask(0755));  // set the correct permissions cause
                                       // it's wrong for some reason
#else
    #error "Platform not implemented!"
#endif
}

#include <iostream>

void FileSystem::Touch(std::string_view pPath)
{
    WriteBinaryFile(pPath, {});
}

void FileSystem::Delete(std::string_view pPath)
{
    remove(pPath.data());
}
