#pragma once

#include "ECS/Entity.h"
#include "ECS/System.h"

#include "Utils/Memory.h"

#include "box2d/box2d.h"

namespace IceSDK::Systems
{
    class PhysicSystem final : public System
    {
    public:
        explicit PhysicSystem(
            const Memory::WeakPtr<entt::registry>& pRegistry) :
            System(pRegistry)
        {
            _world = std::make_shared<b2World>(b2Vec2{ 1.0f, 1.0f });
        }

        void Draw(float pDelta) override;
        void Tick(float pDelta) override;

    private:
        Memory::Ptr<b2World> _world;
    };
}  // namespace IceSDK::Systems
