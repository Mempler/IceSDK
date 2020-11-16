#include "pch.h"

#include "ECS/Systems/CameraSystem.h"

#include "ECS/Components/CameraComponent.h"
#include "ECS/Components/TransformComponent.h"
#include "ECS/Entity.h"

#include "GameBase.h"

using namespace IceSDK;
using namespace IceSDK::Systems;
using namespace IceSDK::Components;

void CameraSystem::Draw(float pDelta)
{
    auto activeCam = CameraSystem::GetActiveCamera(this->_registry);

    if (!Entity::IsValid(activeCam)) return;

    auto& camera = activeCam.GetComponent<CameraComponent>();
    bgfx::setViewTransform(0, glm::value_ptr(camera.view_matrix),
                           glm::value_ptr(camera.projection_matrix));
}

void CameraSystem::Tick(float pDelta)
{
    auto activeCam = CameraSystem::GetActiveCamera(this->_registry);

    if (!Entity::IsValid(activeCam)) return;

    auto& camera = activeCam.GetComponent<CameraComponent>();
    auto& transform = activeCam.GetComponent<TransformComponent>();

    switch (camera.render_mode)
    {
    case eCameraRenderingMode::Orthographic:  // TODO: only execute if value
        // changed
        const auto window = GetWindow();

        camera.screen_size = { window->Width(), window->Height() };

        camera.view_matrix = transform.model_matrix;
        camera.projection_matrix = glm::ortho(
            0.0f, static_cast<float>(window->Width()),
            static_cast<float>(window->Height()), 0.0f, 0.1f, 1000.0f);

        camera.projection_matrix[3].z = 1;  // TODO: Fix
        break;
    }
}

Entity CameraSystem::GetActiveCamera(
    const Memory::WeakPtr<entt::registry> pRegistry)
{
    auto registry = pRegistry.lock();
    if (registry == nullptr) return Entity();

    Entity activeCamera;

    auto cameraGroup = registry->view<Components::CameraComponent>();
    for (auto rawCameraEntity : cameraGroup)
    {
        auto cameraEntity = Entity(pRegistry, rawCameraEntity);

        const auto& cam =
            cameraEntity.GetComponent<Components::CameraComponent>();
        if (cam.is_active)
        {
            activeCamera = cameraEntity;
            break;
        }
    }

    return activeCamera;
}
