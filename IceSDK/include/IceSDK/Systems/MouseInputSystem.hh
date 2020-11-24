#pragma once

#include "IceSDK/System.hh"

#include <memory>

namespace IceSDK::Systems
{
    class MouseInputSystem final : public System
    {
    public:
        explicit MouseInputSystem(
            const std::weak_ptr<entt::registry>& wpRegistry) :
            System(wpRegistry)
        {
        }

        void Tick(float fDelta) override;
        void Draw(float fDelta) override;
    };
}  // namespace IceSDK::Systems
