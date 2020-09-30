#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace IceSDK::Components
{
    struct TransformComponent
    {
        glm::vec3 position;
        glm::vec3 scale;
        float rotation;

        glm::mat4 model_matrix;
    };
}  // namespace IceSDK::Components
