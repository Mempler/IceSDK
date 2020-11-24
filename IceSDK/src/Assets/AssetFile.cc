#include "pch.hh"

#include "Assets/AssetFile.hh"

#include "Utils/FileSystem.hh"
#include "Utils/Instrumentor.hh"
#include "Utils/Logger.hh"

using namespace IceSDK;
using namespace IceSDK::Assets;

bool AssetFile::IsFull() const
{
    return this->GetEmptyEntry() == -1;
}

Asset AssetFile::Get(std::string_view svName)
{
    const auto index = GetIndex(svName);
    if (index == -1) return {};

    return this->Get(static_cast<uint32_t>(index));
}

Asset AssetFile::Get(const uint32_t pIndex)
{
    ICESDK_PROFILE_FUNCTION();
    auto pos = 0;
    for (uint32_t i = 0; i < asset_register_size; ++i)
    {
        if (i == pIndex) break;

        pos += this->header.asset_register[i].size;
    }

    const auto region = this->header.asset_register[pIndex];
    const auto data = std::vector<uint8_t>(
        this->body.begin() + pos, this->body.begin() + pos + region.size);

    return Asset{ region.type, data };
}

int32_t AssetFile::GetIndex(std::string_view svName) const
{
    ICESDK_PROFILE_FUNCTION();
    for (auto i = 0; i < asset_register_size; ++i)
    {
        if (this->header.asset_register[i].name == svName) return i;
    }

    return -1;
}

void AssetFile::Append(std::string_view svName, const Asset& pAsset)
{
    ICESDK_PROFILE_FUNCTION();
    assert(svName.length() <= asset_name_length);

    const auto register_index = GetEmptyEntry();
    if (register_index == -1) return;  // Full TODO: throw an error!

    auto pos = 0;  // Figure out the correct position for the to write to.
    for (const auto& region : this->header.asset_register) pos += region.size;

    auto* region = &this->header.asset_register[register_index];
    memcpy(&region->name[0], &svName[0], svName.size() + 1);

    region->type = pAsset.m_eAssetType;
    region->size = static_cast<uint32_t>(pAsset.m_vData.size());

    this->body.resize(pos + pAsset.m_vData.size());
    memcpy(&this->body[0] + pos, &pAsset.m_vData[0], pAsset.m_vData.size());
}

void AssetFile::Save(std::string_view svPath)
{
    ICESDK_PROFILE_FUNCTION();
    std::vector<uint8_t> data;
    data.resize(sizeof(AssetHeader) + this->body.size());

    memcpy(&data[0], &this->header, sizeof(AssetHeader));
    memcpy(&data[0] + sizeof(AssetHeader), &this->body[0], body.size());

    FileSystem::WriteBinaryFile(svName, data);
}

Memory::Ptr<AssetFile> AssetFile::Load(const std::vector<uint8_t>& pMemory)
{
    ICESDK_PROFILE_FUNCTION();
    auto file = std::make_shared<AssetFile>();  // Our AssetFile is too big to
                                                // be stored in our stack
    {
        memcpy(&file->header, &pMemory[0], sizeof(AssetHeader));

        const auto size =
            -(pMemory.begin() + sizeof(AssetHeader) - pMemory.end());
        file->body.resize(size);
        memcpy(&file->body[0], &pMemory[0] + sizeof(AssetHeader), size);
    }
    return file;
}

Memory::Ptr<AssetFile> AssetFile::Load(std::string_view svPath)
{
    ICESDK_PROFILE_FUNCTION();
    auto data = FileSystem::ReadBinaryFile(svName);

    if (data.empty())
    {
        ICESDK_CORE_ERROR("Failed to load AssetFile: {}!", svName);
        return nullptr;
    }

    ICESDK_CORE_INFO("{}: {}", svName, data.size());

    return AssetFile::Load(data);
}

int32_t AssetFile::GetEmptyEntry() const
{
    ICESDK_PROFILE_FUNCTION();
    for (auto i = 0; i < asset_register_size; ++i)
    {
        if (this->header.asset_register[i].size <= 0) return i;
    }

    return -1;
}
