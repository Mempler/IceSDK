#include "pch.hh"

#include "Scene.hh"

using namespace IceSDK::Core::ECS;

Scene::Scene()
{
    m_pRegistry = std::make_shared<entt::registry>();

    /*
    this->RegisterSystem<Systems::TransformSystem>();
    this->RegisterSystem<Systems::CameraSystem>();

    this->RegisterSystem<Systems::SpriteRenderingSystem>();
    this->RegisterSystem<Systems::TextRenderingSystem>();
    */
}

/*
Entity Scene::CreateEntity(const std::string& pName) const
{
    auto entity = Entity(this->_registry);

    entity.AddComponent<Components::BaseComponent>(pName, true);

    return entity;
}
*/

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
