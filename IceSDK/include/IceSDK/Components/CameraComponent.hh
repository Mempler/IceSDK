#pragma once

#include <glm/fwd.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>

namespace IceSDK::ECS::Components
{
    enum class CameraRenderingMode
    {
        Orthographic,
        // Perspective, // Not supported (yet)
    };

    struct CameraComponent
    {
        // For internal use only
        glm::vec2 _v2OldSize;

        CameraRenderingMode m_eRenderMode;
        bool m_bIsActive;

        // float fov;

        glm::vec2 m_v2ScreenSize;  // 2D screen size

        glm::mat4 m_m4ProjectionMatrix;

        glm::mat4* m_m4ViewMatrix;
    };
}  // namespace IceSDK::ECS::Components
