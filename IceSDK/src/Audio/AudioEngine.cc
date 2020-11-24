#include "pch.h"

#include "Audio/AudioSystem.h"

#include "Utils/FileSystem.h"
#include "Utils/Instrumentor.h"
#include "Utils/Logger.h"

using namespace IceSDK::Audio;

#if defined(ICESDK_FMOD)
FMOD::System* g_FMODSystem = nullptr;
#endif

AudioEngine::AudioSystem()
{
#ifdef ICESDK_FMOD
    assert(g_FMODSystem
           && "AudioSystem::Init() wasn't called or failed! Could not "
              "continue...");
#else
    #warning "No AudioSystem!"
#endif
}

void AudioEngine::Init()
{
    ICESDK_PROFILE_FUNCTION();

#if defined(ICESDK_FMOD)
    if (FMOD::Debug_Initialize(FMOD_DEBUG_LEVEL_LOG))
        ICESDK_CORE_ERROR("Failed to initialize FMOD::Debug");

    if (FMOD::System_Create(&g_FMODSystem) != FMOD_OK)
        ICESDK_CORE_CRITICAL("Failed to create FMOD::System!");

    if (g_FMODSystem->init(2048, FMOD_INIT_NORMAL, nullptr) != FMOD_OK)
        ICESDK_CORE_CRITICAL("Failed to initialize FMOD::System!");
#endif
}

IceSDK::Memory::Ptr<Sound> AudioEngine::Load(const std::string& pPath)
{
    return this->Load(FileSystem::ReadBinaryFile(pPath));
}

IceSDK::Memory::Ptr<Sound> AudioEngine::Load(std::vector<uint8_t> pBuffer)
{
    ICESDK_PROFILE_FUNCTION();

#if defined(ICESDK_FMOD)
    FMOD_CREATESOUNDEXINFO sound_info{};

    auto sound = Memory::Ptr<Sound>(new Sound);

    sound_info.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
    sound_info.length = static_cast<uint32_t>(pBuffer.size());

    const auto result =
        g_FMODSystem->createSound(reinterpret_cast<const char*>(pBuffer.data()),
                                  FMOD_OPENMEMORY, &sound_info, &sound->_sound);
    if (result != FMOD_OK)
    {
        ICESDK_CORE_ERROR("FMOD: {}", FMOD_ErrorString(result));
        return nullptr;
    }

    this->_loaded_audio.push_back(sound);

    return sound;
#endif
}
