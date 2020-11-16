#include "pch.hh"

#include "FileSystem.hh"

#include <fstream>
#include <iostream>
#include <iterator>

#ifdef ICESDK_WIN32
    #include <Windows.h>
#elif defined(ICESDK_LINUX) || defined(ICESDK_ANDROID)
    #include <sys/stat.h>

    #include <dirent.h>
    #include <fcntl.h>
    #include <unistd.h>

    #include <cstdlib>
#elif defined(ICESDK_EMSCRIPTEN)
    #include <sys/stat.h>

    #include <dirent.h>
    #include <emscripten.h>
    #include <stdlib.h>
    #include <unistd.h>

#else
    #error "Platform not implemented!"
#endif

using namespace IceSDK::Core::Utils;

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
bool FileSystem::CreateDirectory(std::string_view svPath)
{
#ifdef ICESDK_WIN32
    return CreateDirectory(svPath.data(), NULL);
#elif defined(ICESDK_LINUX) || defined(ICESDK_EMSCRIPTEN)                      \
    || defined(ICESDK_ANDROID)
    if (mkdir(svPath.data(), umask(0755)))
    {
        chmod(svPath.data(), umask(0755));  // set the correct permissions
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
std::string FileSystem::JoinPath(std::string_view svPath1,
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
bool FileSystem::Exists(std::string_view svPath)
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
bool FileSystem::IsDirectory(std::string_view svPath)
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
std::vector<std::string> FileSystem::ReadDirectory(std::string_view svPath,
                                                   bool bRecursive /*= false */)
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
        do
        {
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

            directories.push_back(FileSystem::JoinPath(svPath, ent->d_name));
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
                { directories.push_back(__dir); }
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
std::string FileSystem::ResolveFullPath(std::string_view svPath)
{
#ifdef ICESDK_WIN32
    char buffer[4096];
    if (IS_ERROR(GetFullPathNameA(svPath.data(), 4096, buffer, nullptr)))
        return "";

    return buffer;
#elif defined(ICESDK_LINUX) || defined(ICESDK_EMSCRIPTEN)                      \
    || defined(ICESDK_ANDROID)
    char buffer[PATH_MAX + 1];

    realpath(svPath.data(), buffer);

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
std::vector<uint8_t> FileSystem::ReadBinaryFile(std::string_view svPath)
{
    std::ifstream file(svPath.data(), std::ios::binary | std::ios::in);
    if (!file && file.fail())
    {
        // TODO: ICESDK_CORE_ERROR("Failed to open {}", svPath);
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

/*****************************************************
 * WriteBinaryFile
 *
 * Writes a binary file
 *
 * @param svPath Input path
 * @param vContent Content
 *****************************************************/
void FileSystem::WriteBinaryFile(std::string_view svPath,
                                 const std::vector<uint8_t>& vContent)
{
#if defined(ICESDK_LINUX) || defined(ICESDK_ANDROID)
    auto fp = open(svPath.data(), O_WRONLY | O_CREAT, umask(0755));
    // TODO: if (fp < 0) ICESDK_CORE_ERROR("Failed to open {}", svPath);

    write(fp, svPath.data(), vContent.size());

    close(fp);
#else
    std::ofstream file(svPath.data(), std::ios::binary | std::ios::out);
    if (!file && file.fail()) ICESDK_CORE_ERROR("Failed to open {}", svPath);

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
void FileSystem::Touch(std::string_view svPath)
{
    WriteBinaryFile(svPath, {});
}

/*****************************************************
 * Delete
 *
 * Deletes a file
 *
 * @param svPath Input path
 *****************************************************/
void FileSystem::Delete(std::string_view svPath)
{
    remove(svPath.data());
}
