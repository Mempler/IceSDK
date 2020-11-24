#pragma once

#include "IceSDK/Entity.hh"
#include "IceSDK/System.hh"

#include <entt/entt.hpp>

#include <memory>

namespace IceSDK::Systems
{
    class CameraSystem final : public System
    {
    public:
        explicit CameraSystem(const std::weak_ptr<entt::registry>& wpRegistry) :
            System(wpRegistry)
        {
        }

        void Draw(float fDelta) override;
        void Tick(float fDelta) override;

        static Entity GetActiveCamera(
            const std::weak_ptr<entt::registry> wpRegistry);  // Helper Function
    };
}  // namespace IceSDK::Systems
