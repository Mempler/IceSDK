#include "PhysicSystem.h"

#include "ECS/Components/TransformComponent.h"

#include "Utils/Instrumentor.h"

#include "Physics/Components/PhysicsComponent.h"
#include "Physics/Components/Rigidbody2D.h"

#include <glm/glm.hpp>

using namespace IceSDK;
using namespace IceSDK::Systems;
using namespace IceSDK::Components;
using namespace IceSDK::Physics::Components;

#define PPM 1.f
#define PHYSICS_SPEED 1.f / 60.f

void PhysicSystem::Draw(float pDelta)
{
    _world->DebugDraw();
}

void PhysicSystem::Tick(float pDelta)
{
    ICESDK_PROFILE_FUNCTION();
    const auto registry = this->_registry.lock();
    if (registry == nullptr) return;

    auto physicsGroup =
        registry->view<PhysicsComponent, RigidBody2DComponent>();
    for (auto rawPhysicsEntity : physicsGroup)
    {
        auto physicsEntity = Entity(this->_registry, rawPhysicsEntity);
        auto& physicsComponent = physicsEntity.GetComponent<PhysicsComponent>();

        if (!physicsComponent.body) return;

        auto& transform = physicsEntity.GetComponent<TransformComponent>();
        transform.position.y = physicsComponent.body->GetPosition().y * PPM;
        transform.position.x = physicsComponent.body->GetPosition().x * PPM;
        transform.rotation = glm::degrees(physicsComponent.body->GetAngle());
    }
}
