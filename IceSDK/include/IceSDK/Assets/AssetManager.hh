#pragma once

#include "IceSDK/Assets/AssetFile.hh"
#include "IceSDK/Audio/Sound.hh"
#include "IceSDK/Graphics/Texture2D.hh"

#include <unordered_map>
#include <vector>

#ifdef ICESDK_EMSCRIPTEN
    #include <emscripten/fetch.h>
#endif

namespace IceSDK::Assets
{
    class AssetManager
    {
    public:
        void Init();  // Load all asset files into memory
                      //	TODO:	Don't do that
                      //	TODO:	make it faster for low end devices!

        Graphics::Texture2D LoadTexture(std::string_view svName);

        Audio::Sound* LoadAudio(std::string_view svName);

        std::string LoadText(std::string_view svName);

        std::vector<AssetFile*>& GetAll();

    private:
        Asset Search(std::string_view svName);

        std::unordered_map<std::string, Asset> m_umSearchCache;
        std::vector<AssetFile*> m_vpAssets;
    };
}  // namespace IceSDK::Assets
