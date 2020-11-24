#pragma once

#include "IceSDK/Assets/AssetHeader.hh"

#include <string>
#include <vector>

namespace IceSDK::Assets
{
    class TextAsset
    {
    public:
        explicit TextAsset(std::string_view svData);

        std::string ToString() const;

        constexpr static AssetType GetAssetType() { return AssetType::Text; }

        std::vector<uint8_t> ToByteArray();
        static TextAsset From(std::string_view svData,
                              const std::vector<uint8_t>& vData);

    private:
        std::string m_sData;
    };
}  // namespace IceSDK::Assets
