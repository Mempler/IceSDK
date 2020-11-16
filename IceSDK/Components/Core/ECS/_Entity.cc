#include "pch.h"

#include "ECS/Entity.h"

#include "ECS/Scene.h"

using namespace IceSDK;

Entity::Entity(const Memory::WeakPtr<entt::registry>& pRegistry)
{
    this->_registry = pRegistry;

    this->_inner = this->_registry.lock()->create();
}

Entity::Entity(const Memory::WeakPtr<entt::registry>& pRegistry,
               const entt::entity pEntity)
{
    this->_registry = pRegistry;
    this->_inner = pEntity;
}

Entity::Entity() { }

bool Entity::IsValid(const Entity& pEntity)
{
    return pEntity._inner != entt::null;
}
