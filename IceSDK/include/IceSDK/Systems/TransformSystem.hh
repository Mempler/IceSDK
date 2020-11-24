#pragma once

#include "IceSDK/System.hh"

namespace IceSDK::Systems
{
    class TransformSystem final : public System
    {
    public:
        explicit TransformSystem(
            const std::weak_ptr<entt::registry>& wpRegistry) :
            System(wpRegistry)
        {
        }

        void Draw(float fDelta) override;
        void Tick(float fDelta) override;
    };
}  // namespace IceSDK::Systems
