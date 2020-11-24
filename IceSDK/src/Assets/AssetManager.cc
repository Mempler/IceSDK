#include "pch.h"

#include "Assets/AssetManager.h"

#include "Assets/AudioAsset.h"
#include "Assets/TextAsset.h"
#include "Assets/Texture2DAsset.h"

#include "Utils/FileSystem.h"
#include "Utils/Instrumentor.h"
#include "Utils/Logger.h"

using namespace IceSDK;
using namespace IceSDK::Assets;

void AssetManager::Init()
{
    ICESDK_PROFILE_FUNCTION();
    if (!FileSystem::Exists("./Assets")) return;

    for (const auto& assetFilePath : FileSystem::ReadDirectory("./Assets"))
    {
        if (FileSystem::IsDirectory(assetFilePath)) continue;

        auto assetFile = AssetFile::Load(assetFilePath);
        if (assetFile == nullptr)
        {
            ICESDK_CORE_CRITICAL("Failed to initialize AssetManager");
            break;
        }

        this->_assets.push_back(assetFile);
    }
}

Memory::Ptr<Graphics::Texture2D> AssetManager::LoadTexture(
    std::string_view svName)
{
    auto asset = this->Search(svName);
    if (asset.data.empty())  // Null objects aren't acceptable.
        return nullptr;

    return asset.Into<Texture2DAsset>(svName).LoadTexture();
}

Memory::Ptr<Audio::Sound> AssetManager::LoadAudio(std::string_view svName)
{
    auto asset = this->Search(svName);

    return asset.Into<AudioAsset>(svName).LoadAudio();
}

std::string AssetManager::LoadText(std::string_view svName)
{
    auto asset = this->Search(svName);

    return asset.Into<TextAsset>(svName).ToString();
}

Asset AssetManager::Search(std::string_view svName)
{
    if (this->_search_cache.count(svName)) return this->_search_cache[svName];

    for (auto assetFile : this->_assets)
    {
        auto asset = assetFile->Get(svName);
        if (asset.asset_type != eAssetType::Unknown)
        {
            this->_search_cache[svName] = asset;
            return asset;
        }
    }

    this->_search_cache[svName] = {};  // Insert empty key

    return {};
}

std::vector<Memory::Ptr<AssetFile>>& AssetManager::GetAll()
{
    return this->_assets;
}
