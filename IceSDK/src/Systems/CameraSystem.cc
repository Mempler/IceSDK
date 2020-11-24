#include "pch.hh"

#include "ECS/Systems/CameraSystem.hh"

#include "ECS/Components/CameraComponent.hh"
#include "ECS/Components/TransformComponent.hh"
#include "ECS/Entity.hh"

#include "GameBase.h"
#include "GameBase.hh"

#include <memory>

using namespace IceSDK;
using namespace IceSDK::ECS;
using namespace IceSDK::ECS::Systems;
using namespace IceSDK::ECS::Components;

void CameraSystem::Draw(float fDelta)
{
    auto activeCam = CameraSystem::GetActiveCamera(m_wpRegistry);

    if (!Entity::IsValid(activeCam)) return;

    auto& camera = activeCam.GetComponent<CameraComponent>();
    bgfx::setViewTransform(0, glm::value_ptr(camera.view_matrix),
                           glm::value_ptr(camera.projection_matrix));
}

void CameraSystem::Tick(float fDelta)
{
    auto activeCam = CameraSystem::GetActiveCamera(m_wpRegistry);

    if (!Entity::IsValid(activeCam)) return;

    auto& camera = activeCam.GetComponent<CameraComponent>();
    auto& transform = activeCam.GetComponent<TransformComponent>();

    const auto window = GetGameBase()->GetGameWindow();

    // Matrx calculations are expensive, so we're going to update if a value
    // changed

    auto screenSize = glm::vec2{ window->Width(), window->Height() };
    if (camera.screen_size != screenSize)
    {
        switch (camera.render_mode)
        {
        case CameraRenderingMode::Orthographic:
            camera.screen_size = screenSize;

            camera.projection_matrix = glm::ortho(
                0.0f, screenSize.x, screenSize.y, 0.0f, 0.1f, 1000.0f);

            camera.projection_matrix[3].z = 1;  // TODO: Fix
            break;
        }
    }

    // Removes the need for fetching a TransformationComponent (slow!)
    camera.view_matrix = &transform.model_matrix;
}

Entity CameraSystem::GetActiveCamera(
    const std::weak_ptr<entt::registry> wpRegistry)
{
    auto registry = wpRegistry.lock();
    if (registry == nullptr) return Entity();

    Entity activeCamera;

    auto cameraGroup = registry->view<Components::CameraComponent>();
    for (auto rawCameraEntity : cameraGroup)
    {
        auto cameraEntity = Entity(wpRegistry, rawCameraEntity);

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
