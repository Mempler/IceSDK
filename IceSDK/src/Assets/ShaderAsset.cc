#include "pch.h"

#include "Assets/ShaderAsset.h"

#include "Utils/Instrumentor.h"

using namespace IceSDK;
using namespace IceSDK::Assets;

/*
uint8_t (Amount of Shaders)
uint16_t[] (Platform | Type)

uint8_t[][] (Buffer)
*/
std::vector<uint8_t> ShaderAsset::ToByteArray() const
{
    ICESDK_PROFILE_FUNCTION();
    size_t offset = 0;
    std::vector<uint8_t> buffer;
    buffer.resize(1);

    uint8_t shaderSize = this->_shaders.size();
    memcpy(buffer.data(), &shaderSize, 1);
    offset += 1;

    buffer.resize(offset + static_cast<size_t>(shaderSize) * 2);
    for (const auto& shader : this->_shaders)
    {
        memcpy(buffer.data() + offset, &shader, 2);

        offset += 2;
    }

    for (const auto& shader : this->_shaders)
    {
        uint32_t shaderSize = shader.code.size();

        buffer.resize(offset + 4);
        memcpy(buffer.data() + offset, &shaderSize, 4);
        offset += 4;

        buffer.reserve(shader.code.size());
        for (auto code : shader.code) buffer.push_back(code);

        offset += shader.code.size();
    }

    return buffer;
}

ShaderAsset ShaderAsset::From(std::string pName, std::vector<uint8_t> pData)
{
    ICESDK_PROFILE_FUNCTION();
    ShaderAsset shaderAsset;

    uint8_t shaderSize = 0;
    memcpy(&shaderSize, pData.data(), 1);

    auto offset = 1;

    for (auto i = 0; i < shaderSize; ++i)
    {
        Shader shader{};

        memcpy(&shader.platform, pData.data() + offset, 1);
        memcpy(&shader.type, pData.data() + 1 + offset, 1);

        offset += 2;

        shaderAsset._shaders.push_back(shader);
    }

    for (auto shader : shaderAsset._shaders)
    {
        uint32_t shaderSize = 0;
        memcpy(&shaderSize, pData.data(), 4);

        offset += 4;

        shader.code.resize(shaderSize);
        memcpy(shader.code.data(), pData.data() + offset, shaderSize);

        offset += shaderSize;
    }

    return shaderAsset;
}
