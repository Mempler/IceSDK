#pragma once

#include <cstdint>

namespace IceFS
{
    enum class EntryType
    {
        Folder,  // a Folder is technically another IceFS
        File,
        Link,  // Hard link
    };

    enum EntryFlags : uint8_t
    {
        None = 1 << 0,
        Compressed = 1 << 1
    };

    struct EntryHeader
    {
        uint8_t Flags = EntryFlags::None;

        uint64_t BodyChecksum = 0;  // Decompressed body checksum.
        uint32_t BodyPointer = 0;
        uint32_t BodySize = 0;

        // Padding to make each header exactly 64b
        uint8_t _[0x40 - 1 - 4 - 4 - 8];
    };
}  // namespace IceFS
