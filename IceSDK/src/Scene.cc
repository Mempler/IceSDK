#include "pch.hh"

#include "Scene.h"

#include "Components/BaseComponent.h"
#include "Components/CameraComponent.h"
#include "Components/TransformComponent.h"
#include "Systems/CameraSystem.h"
#include "Systems/TransformSystem.h"

#include "Graphics/Systems/SpriteRenderingSystem.h"
#include "Graphics/Systems/TextRenderingSystem.h"

using namespace IceSDK;

Scene::Scene()
{
    m_pRegistry = std::make_shared<entt::registry>();

    RegisterSystem<Systems::TransformSystem>();
    RegisterSystem<Systems::CameraSystem>();

    RegisterSystem<Systems::SpriteRenderingSystem>();
    RegisterSystem<Systems::TextRenderingSystem>();
}

/*****************************************************
 * CreateEntity
 *
 * Creates an Entity which components can be attached
 * to. It'll be used to interact with the Engine
 * and non Engine systems
 *
 * @param svName Name of the Entity
 *
 * @return Entity
 *****************************************************/
Entity CreateEntity(std::string_view svName) const
{
    auto entity = Entity(m_pRegistry);

    entity.AddComponent<Components::BaseComponent>(svName, true);

    return entity;
}

/*****************************************************
 * Tick
 *
 * Tick the underlying systems
 *
 * @param fDelta Delta time between the last tick
 *****************************************************/
void Scene::Tick(float fDelta)
{
    for (auto system : m_vpSystems) { system->Tick(fDelta); }
}

/*****************************************************
 * Draw
 *
 * Draws the underlying systems
 *
 * @param fDelta Delta time between the last draw
 *****************************************************/
void Scene::Draw(float fDelta)
{
    for (auto system : m_vpSystems) { system->Draw(fDelta); }
}
