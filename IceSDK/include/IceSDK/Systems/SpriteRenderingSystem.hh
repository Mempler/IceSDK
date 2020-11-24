#pragma once

#include "IceSDK/System.hh"

#include <memory>

namespace IceSDK::Systems
{
    class SpriteRenderingSystem final : public System
    {
    public:
        explicit SpriteRenderingSystem(
            const std::weak_ptr<entt::registry>& wpRegistry) :
            System(wpRegistry)
        {
        }

        static void Init();

        void Tick(float fDelta) override;
        void Draw(float fDelta) override;
    };
}  // namespace IceSDK::Systems
