#pragma once

#include "ECS/Entity.h"
#include "ECS/Scene.h"

#include "Graphics/Fonts/FontManager.h"
#include "Graphics/Shaders/ShaderManager.h"
#include "Graphics/Texture2D.h"

#include <glm/glm.hpp>

namespace IceSDK::Graphics::Entity
{
    void Init(
        const Memory::Ptr<Graphics::Shaders::ShaderManager>& pShaderManager);
    void InitScene(const Memory::Ptr<IceSDK::Scene>& pScene);

    IceSDK::Entity CreateSprite(
        Memory::Ptr<IceSDK::Scene> pScene,
        Memory::Ptr<Shaders::ShaderManager> pShaderManager,
        Memory::Ptr<Texture2D> pTex,
        const glm::vec3& pPosition = { 0.f, 0.f, 0.f },
        const glm::vec2& pSize = { -1.f, -1.f }, const glm::vec4& pTileinfo = { -1.f, -1.f, -1.f, -1.f }, float pRotation = 0.0f);

    IceSDK::Entity CreateText(
        Memory::Ptr<IceSDK::Scene> pScene,
        Memory::Ptr<Shaders::ShaderManager> pShaderManager,
        const std::string& pText, size_t pFontSize, FontFaceHandle pFontFace);
}  // namespace IceSDK::Graphics::Entity
