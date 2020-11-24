#pragma once

#include "IceSDK/Graphics/Texture2D.hh"

#include <glm/vec2.hpp>

namespace IceSDK::Components
{
    struct SpriteComponent
    {
        glm::vec2 m_v2Size;

        Graphics::Texture2D m_Texture2D;
    };
}  // namespace IceSDK::Components
