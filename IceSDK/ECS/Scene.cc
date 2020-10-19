#include "pch.h"

#include "ECS/Scene.h"

#include "ECS/Components/BaseComponent.h"
#include "ECS/Components/CameraComponent.h"
#include "ECS/Components/TransformComponent.h"
#include "ECS/Systems/CameraSystem.h"
#include "ECS/Systems/TransformSystem.h"

#include "Utils/Instrumentor.h"

using namespace IceSDK;

Scene::Scene()
{
    ICESDK_PROFILE_FUNCTION();

    this->_registry = std::make_shared<entt::registry>();

    CreateCamera();

    this->RegisterSystem<Systems::TransformSystem>();

    this->RegisterSystem<Systems::CameraSystem>();
}

Entity Scene::CreateEntity(const std::string& pName) const
{
    ICESDK_PROFILE_FUNCTION();

    auto entity = Entity(this->_registry);

    entity.AddComponent<Components::BaseComponent>(pName, true);

    return entity;
}

Entity Scene::CreateCamera() const
{
    ICESDK_PROFILE_FUNCTION();

    auto entity = this->CreateEntity("Camera");

    entity.AddComponent<Components::TransformComponent>(
        glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 1.0f, 1.0f, 1.0f });
    entity.AddComponent<Components::CameraComponent>(
        Components::eCameraRenderingMode::Orthographic, true, glm::vec2(0, 0),
        glm::mat4(1), glm::mat4(1));

    return entity;
}

void Scene::Tick(const float pDelta)
{
    ICESDK_PROFILE_FUNCTION();

    this->_mut_systems.lock();
    for (auto system : this->_systems)
    {
        if (system != nullptr) system->Tick(pDelta);
    }
    this->_mut_systems.unlock();
}

void Scene::Draw(const float pDelta)
{
    ICESDK_PROFILE_FUNCTION();

    for (auto system : this->_systems)
    {
        if (system != nullptr) system->Draw(pDelta);
    }
}

Memory::WeakPtr<entt::registry> Scene::GetRegistry()
{
    return this->_registry;
}
