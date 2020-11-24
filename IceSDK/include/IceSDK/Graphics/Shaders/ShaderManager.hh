#pragma once

#include <bgfx/bgfx.h>

#include <string>
#include <unordered_map>
#include <vector>

namespace IceSDK::Graphics::Shaders
{
    enum class ShaderType : uint8_t
    {
        Fragment = 0,
        Vertex = 1
    };

    class ShaderManager
    {
    public:
        // Internal function, can be used but shouldn't! use at your own risk!
        void AppendShader(const std::string& sKey,
                          const bgfx::RendererType::Enum eRenderer,
                          const ShaderType eType,
                          const std::vector<uint8_t>& vSrc);

        // Internal function, can be used but shouldn't! use at your own risk!
        bgfx::ShaderHandle LoadShader(const std::string& sKey,
                                      const ShaderType eShaderType);

        bgfx::ProgramHandle LoadProgram(const std::string& sKey);

    private:
        struct Shader
        {
            std::string m_sName;
            bgfx::RendererType::Enum m_eRenderer;
            ShaderType m_eType;

            std::vector<uint8_t> m_vCode;
        };

        std::unordered_map<std::string, bgfx::ProgramHandle> m_umPrograms;

        std::vector<Shader> m_vShaders;
    };
}  // namespace IceSDK::Graphics::Shaders
