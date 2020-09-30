#include "pch.h"

#include "ECS/Systems/TransformSystem.h"

#include "ECS/Components/TransformComponent.h"
#include "ECS/Entity.h"

using namespace IceSDK;
using namespace IceSDK::Systems;
using namespace IceSDK::Components;

void TransformSystem::Draw(float pDelta) { }

void TransformSystem::Tick(float pDelta)
{
    const auto registry = this->_registry.lock();
    if (registry == nullptr) return;

    auto transformGroup = registry->view<TransformComponent>();

    for (auto rawTransformEntity : transformGroup)
    {
        auto spriteEntity = Entity(this->_registry, rawTransformEntity);
        auto& transform = spriteEntity.GetComponent<TransformComponent>();

        glm::mat4 matrix =
            glm::translate(glm::mat4(1.0f), transform.position)
            * glm::rotate(glm::mat4(1.0f), glm::radians(transform.rotation),
                glm::vec3(0.f, 0.f, 1.f))
            * glm::scale(glm::mat4(1.0f), transform.scale);

        transform.model_matrix = matrix;
    }
}
