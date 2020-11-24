#pragma once

#include "IceSDK/Assets/AssetHeader.hh"
#include "IceSDK/Audio/AudioEngine.hh"

#include <vector>

namespace IceSDK::Assets
{
    class AudioAsset
    {
    public:
        explicit AudioAsset(const std::vector<uint8_t>& vBuffer);

        Audio::Sound* LoadAudio() const;

        constexpr static AssetType GetAssetType() { return AssetType::Audio; }

        std::vector<uint8_t> ToByteArray() const;
        static AudioAsset From(std::string_view svName,
                               const std::vector<uint8_t>& vBuffer);

    private:
        std::vector<uint8_t> m_vData{};
    };
}  // namespace IceSDK::Assets
