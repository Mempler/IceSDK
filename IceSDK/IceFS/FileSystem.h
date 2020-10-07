#pragma once

#include <string_view>
#include <unordered_map>

#include <tuple>
#include <vector>

namespace IceFS
{
    enum class FSError
    {
        NotFound,
        Corrupted,
        Encrypted  // File is encrypted but we failed to decrypt
    };

    /*
        Entry Headers []
        Entry Data
    */

    class FileSystem
    {
    public:
        FileSystem(std::string_view pPath);

        // Deconstruct will defrag && flush to disk if changed
        ~FileSystem();

        void Write(std::string_view pPath, const std::vector<uint8_t>& pData);
        std::vector<uint8_t> Read(std::string_view pPath);

        void Delete(std::string_view pPath);

        void CreateDir(std::string_view pPath);
        void ReadDir(std::string_view pPath);

        void Copy(std::string_view pPath, std::string_view pTargetPath);

        // Get the last FS error
        FSError GetLastError();

        // Defrag the File System cause each write makes the file bigger due to
        // fragmentation
        // inMemory = make sure we defrag everything in memory before writting
        // to disk else = do everything on the disk (this is much slower)
        void Defrag(bool inMemory = false);

        // Flush changes to disk. This won't defrag.
        void Flush();

    private:
    };

}  // namespace IceFS
