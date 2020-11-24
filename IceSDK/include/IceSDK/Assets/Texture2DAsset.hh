#pragma once

#include "IceSDK/Assets/Asset.hh"
#include "IceSDK/Graphics/Texture2D.hh"

#include <cstdint>

namespace IceSDK::Assets
{
    class Texture2DAsset
    {
    public:
        explicit Texture2DAsset(std::string_view svName,
                                const std::vector<uint8_t>& vData);

        Graphics::Texture2D* LoadTexture() const;

        constexpr static AssetType GetAssetType() { return AssetType::Texture; }

        std::vector<uint8_t> ToByteArray() const;
        static Texture2DAsset From(std::string_view svName,
                                   const std::vector<uint8_t>& vData);

    private:
        std::string m_sName;
        std::vector<uint8_t> m_vData;
    };
}  // namespace IceSDK::Assets
