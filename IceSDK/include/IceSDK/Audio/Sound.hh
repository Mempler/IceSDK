#pragma once

#include <cstdint>

#if defined(ICESDK_FMOD)
    #include <fmod.hpp>
#endif

namespace IceSDK
{
    namespace Audio
    {
        class AudioSystem;

        class Sound
        {
        public:
            ~Sound();

            void Play();
            void Pause(bool bIsPaused = true) const;
            void Stop();
            void SetVolume(float fVolume = 1.0f) const;

            void SetFrequencyMultiplier(float fMultiplier = 1.0f);
            void Seek(uint32_t uPosition) const;
            void Reset();
            uint32_t Length() const;

        private:
            float _frequency;

            explicit Sound() = default;

#if defined(ICESDK_FMOD)
            FMOD::Sound* m_pSound = nullptr;
            FMOD::Channel* m_pSoundChannel = nullptr;
#endif

            friend AudioSystem;
        };
    }  // namespace Audio
}  // namespace IceSDK
