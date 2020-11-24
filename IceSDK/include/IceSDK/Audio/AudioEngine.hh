#pragma once

#include "IceSDK/Audio/AudioEnums.hh"
#include "IceSDK/Audio/Sound.hh"

#include <string>
#include <vector>

#ifdef ICESDK_FMOD
extern FMOD::System* g_FMODSystem;
#endif

namespace IceSDK::Audio
{
    class AudioEngine
    {
    public:
        explicit AudioEngine();

        static void Init();

        Sound* Load(const std::string& pPath);
        Sound* Load(const std::vector<uint8_t>& pBuffer);

        void CreateGroup(const std::string& pName);

    private:
        std::vector<Sound*> m_vpLoadedAudio;
        AudioEngineType m_AudioEngineType = AudioEngineType::FMOD;

        friend Sound;
    };
}  // namespace IceSDK::Audio
