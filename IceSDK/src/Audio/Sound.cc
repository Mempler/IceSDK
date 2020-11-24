#include "pch.h"

#include "Audio/Sound.h"

#include "Audio/AudioSystem.h"

#include "Utils/Instrumentor.h"
#include "Utils/Logger.h"

using namespace IceSDK::Audio;

Sound::~Sound()
{
#if defined(ICESDK_FMOD)
    auto* channel = static_cast<FMOD::Channel*>(this->_sound_channel);
    if (channel != nullptr)
    {
        channel->stop();
        this->_sound_channel = nullptr;
    }
#endif
}

void Sound::Play()
{
    ICESDK_PROFILE_FUNCTION();

    this->_frequency = 0.0;

#if defined(ICESDK_FMOD)
    if (this->_sound_channel != nullptr)
        this->Stop();  // Stop before we play... prevents a memory leak

    const auto result = g_FMODSystem->playSound(this->_sound, nullptr, false,
                                                &this->_sound_channel);
    if (result != FMOD_OK)
        ICESDK_CORE_ERROR("FMOD: {}", FMOD_ErrorString(result));
#endif
}

void Sound::Pause(const bool pIsPaused) const
{
    ICESDK_PROFILE_FUNCTION();

#if defined(ICESDK_FMOD)
    if (this->_sound_channel == nullptr) return;

    this->_sound_channel->setPaused(pIsPaused);
#endif
}

void Sound::Stop()
{
    ICESDK_PROFILE_FUNCTION();
#if defined(ICESDK_FMOD)
    if (this->_sound_channel == nullptr) return;

    this->_sound_channel->stop();

    this->_sound_channel = nullptr;
#endif
}

void Sound::SetVolume(const float pVolume) const
{
    ICESDK_PROFILE_FUNCTION();
#if defined(ICESDK_FMOD)
    if (this->_sound_channel == nullptr) return;

    this->_sound_channel->setVolume(pVolume);
#endif
}

void Sound::SetFrequencyMultiplier(const float pMultiplier)
{
    ICESDK_PROFILE_FUNCTION();
#if defined(ICESDK_FMOD)
    if (this->_sound_channel == nullptr) return;

    if (this->_frequency == 0.0f && pMultiplier != 1.0f)
        this->_sound_channel->getFrequency(&this->_frequency);
    else
        this->_frequency = 0.0f;

    this->_sound_channel->setFrequency(this->_frequency * pMultiplier);
#endif
}

void Sound::Seek(const uint32_t pPosition) const
{
    ICESDK_PROFILE_FUNCTION();
#if defined(ICESDK_FMOD)
    if (this->_sound_channel == nullptr) return;

    this->_sound_channel->setPosition(pPosition, FMOD_TIMEUNIT_MS);
#endif
}

void Sound::Reset()
{
    ICESDK_PROFILE_FUNCTION();
    this->Stop();
    this->_frequency = 0.0;
}

uint32_t Sound::Length() const
{
    ICESDK_PROFILE_FUNCTION();
#if defined(ICESDK_FMOD)
    if (this->_sound == nullptr) return 0;

    uint32_t length;
    this->_sound->getLength(&length, FMOD_TIMEUNIT_MS);

    return length;
#endif
}
