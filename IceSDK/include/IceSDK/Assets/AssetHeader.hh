#pragma once

#include <cstdint>

namespace IceSDK::Assets
{
    constexpr auto asset_magic_value = 0x53464C45584950;
    constexpr auto asset_version = 0x0001;
    constexpr auto asset_register_size = 256;
    constexpr auto asset_name_length = 128;

    enum class AssetType : uint8_t
    {
        Unknown = 0,

        Texture = 1,
        AnimatedTexture = 2,
        Audio = 3,
        Video = 4,
        Particle = 5,

        Shader = 6,

        Text = 7
    };

    struct AssetRegion
    {
        AssetType m_eType = AssetType::Unknown;
        uint32_t m_iSize = 0;
        char m_cName[asset_name_length]{};
    };

    struct AssetHeader
    {
        uint64_t m_uMagicValue = asset_magic_value;
        uint16_t m_uVersion = asset_version;

        AssetRegion m_AssetRegister[asset_register_size];
    };
}  // namespace IceSDK::Assets
