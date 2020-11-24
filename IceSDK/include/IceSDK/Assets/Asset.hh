#pragma once

#include "IceSDK/Assets/AssetHeader.hh"

#include <cstdint>
#include <string>
#include <vector>

namespace IceSDK::Assets
{
    struct Asset
    {
        AssetType m_eAssetType = AssetType::Unknown;
        std::vector<uint8_t> m_vData{};

        Asset() { }

        Asset(AssetType eType, const std::vector<uint8_t>& vData) :
            m_eAssetType(eType), m_vData(vData)
        {
        }

        /*****************************************************
         * Into
         *
         * Converts into the givem IAsset Type.
         *
         * @param svName Asset Name
         *
         * @return IAsset
         *****************************************************/
        template<typename IAsset>
        IAsset Into(std::string_view svName)
        {
            return IAsset::From(svName, this->m_vData);
        }

        template<typename IAsset>
        static Asset From(IAsset pTo)
        {
            Asset asset;

            asset.m_eAssetType = IAsset::GetAssetType();
            asset.m_vData = pTo.ToByteArray();

            return asset;
        }
    };
}  // namespace IceSDK::Assets
