#include "pch.h"

#include "Graphics/Shaders/ShaderManager.h"

#include "Utils/Instrumentor.h"

using namespace IceSDK;
using namespace IceSDK::Graphics;
using namespace IceSDK::Graphics::Shaders;

void ShaderManager::AppendShader(const std::string& pKey,
                                 const bgfx::RendererType::Enum pRenderer,
                                 const eShaderType pType,
                                 const std::vector<uint8_t>& pSrc)
{
    ICESDK_PROFILE_FUNCTION();
    this->_shaders.push_back(Shader{ pKey, pRenderer, pType, pSrc });
}

bgfx::ShaderHandle ShaderManager::LoadShader(const std::string& pKey,
                                             const eShaderType pShaderType)
{
    ICESDK_PROFILE_FUNCTION();
    Shader* p_shader = nullptr;

    for (auto& shader : this->_shaders)
    {
        if (shader.name == pKey && shader.type == pShaderType
            && shader.renderer == bgfx::getRendererType())
        {
            p_shader = &shader;
            break;
        }
    }

    if (p_shader == nullptr) return BGFX_INVALID_HANDLE;

    const auto* buffer = bgfx::copy(
        p_shader->code.data(), static_cast<uint32_t>(p_shader->code.size()));

    const auto handle = bgfx::createShader(buffer);
    bgfx::setName(handle, pKey.c_str());

    return handle;
}

bgfx::ProgramHandle ShaderManager::LoadProgram(const std::string& pKey)
{
    ICESDK_PROFILE_FUNCTION();
    if (_programs.find(pKey) != _programs.end()) return _programs[pKey];

    const auto vsh = this->LoadShader(pKey, eShaderType::Vertex);
    const auto fsh = this->LoadShader(pKey, eShaderType::Fragment);

    const auto handle = bgfx::createProgram(vsh, fsh, true);

    _programs[pKey] = handle;

    return handle;
}
