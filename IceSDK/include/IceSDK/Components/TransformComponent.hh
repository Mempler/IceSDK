#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace IceSDK::Components
{
    struct TransformComponent
    {
        glm::vec3 m_v3Position;
        glm::vec3 m_v3Scale;
        float m_fRotation;

        glm::mat4 m_m4ModelMatrix;
    };
}  // namespace IceSDK::Components
