#pragma once

#include "IceSDK/Assets/Asset.hh"
#include "IceSDK/Assets/AssetHeader.hh"

#include <string>
#include <vector>

namespace IceSDK::Assets
{
    struct AssetFile
    {
        AssetHeader m_aHeader;
        std::vector<uint8_t> m_vBody;

        bool IsFull() const;

        template<typename IAsset>
        IAsset Get(std::string_view svName)
        {
            return this->Get(svName).Into<IAsset>(svName);
        }
        template<typename IAsset>
        IAsset Get(const std::uint32_t uIndex)
        {
            return this->Get(uIndex).Into<IAsset>(
                this->m_aHeader.m_AssetRegister[uIndex].m_cName);
        }

        Asset Get(std::string_view svName);
        Asset Get(uint32_t uIndex);

        int32_t GetIndex(std::string_view svName) const;

        template<typename T>
        void Append(std::string_view svName, T tAsset)
        {
            this->Append(svName, Asset::From<T>(tAsset));
        }
        void Append(std::string_view svName, const Asset& aAsset);

        void Save(std::string_view svPath);
        static AssetFile* Load(std::string_view svPath);
        static AssetFile* Load(const std::vector<uint8_t>& vBuffer);

    private:
        int32_t GetEmptyEntry() const;
    };
}  // namespace IceSDK::Assets
