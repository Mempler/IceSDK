#pragma once

#include "ECS/System.h"

namespace IceSDK::Systems
{
    class TransformSystem final : public System
    {
    public:
        explicit TransformSystem(
            const Memory::WeakPtr<entt::registry>& pRegistry) :
            System(pRegistry)
        {
        }

        void Draw(float pDelta) override;
        void Tick(float pDelta) override;
    };
}  // namespace IceSDK::Systems
