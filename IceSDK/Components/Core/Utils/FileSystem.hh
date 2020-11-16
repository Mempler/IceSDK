#pragma once

#include <string_view>

#include <string>
#include <vector>

namespace IceSDK::Core::Utils::FileSystem
{
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
    bool CreateDirectory(std::string_view svPath);

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
    std::string JoinPath(std::string_view svPath1, std::string_view svPath2);

    /*****************************************************
     * Exists
     *
     * Checks if a Path exists
     *
     * @param svPath Input path
     *
     * @return true if exists
     *****************************************************/
    bool Exists(std::string_view sv_Path);

    /*****************************************************
     * IsDirectory
     *
     * Checks if a Path is a Directory
     *
     * @param svPath Input path
     *
     * @return true if is a directory
     *****************************************************/
    bool IsDirectory(std::string_view svPath);

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
    std::vector<std::string> ReadDirectory(std::string_view svPath,
                                           bool bRecursive = false);

    /*****************************************************
     * ResolveFullPath
     *
     * Resolves a full path of an directory
     *
     * @param svPath Input path
     *
     * @return full path
     *****************************************************/
    std::string ResolveFullPath(std::string_view svPath);

    /*****************************************************
     * ReadBinaryFile
     *
     * Reads a binary file and returns it's content
     *
     * @param svPath Input path
     *
     * @return content
     *****************************************************/
    std::vector<uint8_t> ReadBinaryFile(std::string_view svPath);

    /*****************************************************
     * WriteBinaryFile
     *
     * Writes a binary file
     *
     * @param svPath Input path
     * @param vContent Content
     *****************************************************/
    void WriteBinaryFile(std::string_view svPath,
                         const std::vector<uint8_t>& vContent);

    /*****************************************************
     * Touch
     *
     * Creates an empty file
     *
     * @param svPath Input path
     *****************************************************/
    void Touch(std::string_view svPath);

    /*****************************************************
     * Delete
     *
     * Deletes a file
     *
     * @param svPath Input path
     *****************************************************/
    void Delete(std::string_view svPath);
}  // namespace IceSDK::Core::Utils::FileSystem
