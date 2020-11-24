#pragma once

#include "IceSDK/Assets/Asset.hh"
#include "IceSDK/Graphics/Shaders/ShaderManager.hh"

#include <bgfx/bgfx.h>

#include <vector>

namespace IceSDK::Assets
{
    class ShaderAsset
    {
    public:
        // Memory::Ptr<Audio::Audio> LoadAudio() const;

        constexpr static AssetType GetAssetType() { return AssetType::Shader; }

        std::vector<uint8_t> ToByteArray() const;
        static ShaderAsset From(std::string_view svName,
                                const std::vector<uint8_t>& vBuffer);

    private:
        struct Shader
        {
            Graphics::Shaders::ShaderType m_eType;
            bgfx::RendererType::Enum m_ePlatform;
            std::vector<uint8_t> m_vCode;
        };

        std::vector<Shader> m_vShaders{};
    };
}  // namespace IceSDK::Assets
