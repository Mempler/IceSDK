#pragma once

#include "ECS/System.h"

namespace IceSDK::Systems
{
    class SpriteRenderingSystem final : public System
    {
    public:
        explicit SpriteRenderingSystem(
            const Memory::WeakPtr<entt::registry>& pRegistry) :
            System(pRegistry)
        {
        }

        static void Init();

        void Tick(float pDelta) override;
        void Draw(float pDelta) override;
    };
}  // namespace IceSDK::Systems
