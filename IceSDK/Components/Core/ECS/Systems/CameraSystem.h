#pragma once

#include "ECS/Entity.h"
#include "ECS/System.h"

namespace IceSDK::Systems
{
    class CameraSystem final : public System
    {
    public:
        explicit CameraSystem(
            const Memory::WeakPtr<entt::registry>& pRegistry) :
            System(pRegistry)
        {
        }

        void Draw(float pDelta) override;
        void Tick(float pDelta) override;

        static Entity GetActiveCamera(const Memory::WeakPtr<entt::registry>
                                          pRegistry);  // Helper Function
    };
}  // namespace IceSDK::Systems
