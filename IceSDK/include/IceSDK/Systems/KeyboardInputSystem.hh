#pragma once

#include "IceSDK/System.hh"

namespace IceSDK::Systems
{
    class KeyboardInputSystem final : public System
    {
    public:
        explicit KeyboardInputSystem(
            const std::weak_ptr<entt::registry>& fRegistry) :
            System(fRegistry)
        {
        }

        void Tick(float fDelta) override;
        void Draw(float fDelta) override;
    };
}  // namespace IceSDK::Systems
