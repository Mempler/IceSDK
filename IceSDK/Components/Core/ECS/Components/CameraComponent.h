#pragma once

#include <glm/fwd.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>

namespace IceSDK::Components
{
    enum class eCameraRenderingMode
    {
        Orthographic,
        // Perspective, // Not supported (yet)
    };

    struct CameraComponent
    {
        eCameraRenderingMode render_mode;
        bool is_active;

        // float fov;

        glm::vec2 screen_size;  // 2D screen size

        glm::mat4 view_matrix;
        glm::mat4 projection_matrix;
    };
}  // namespace IceSDK::Components
