#include "pch.h"

#include "Assets/AudioAsset.h"

using namespace IceSDK;
using namespace IceSDK::Assets;

Memory::Ptr<Audio::AudioSystem> GetAudioSystem();

AudioAsset::AudioAsset(std::vector<uint8_t> pData) : _data(std::move(pData)) { }

Memory::Ptr<Audio::Sound> AudioAsset::LoadAudio() const
{
    return GetAudioSystem()->Load(this->_data);
}

std::vector<uint8_t> AudioAsset::ToByteArray() const
{
    return this->_data;
}

AudioAsset AudioAsset::From(std::string pName, std::vector<uint8_t> pData)
{
    return AudioAsset{ pData };
}
