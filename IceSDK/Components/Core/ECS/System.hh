#pragma once

#include <entt/entt.hpp>

#include <memory>

namespace IceSDK::Core::ECS
{
    class System
    {
    protected:
        std::weak_ptr<entt::registry> m_wpRegistry;

    public:
        explicit System(const std::weak_ptr<entt::registry>& wpRegistry);
        virtual ~System() = default;

        /*****************************************************
         * Tick
         *
         * Tick the current system
         *
         * @param fDelta Delta time between the last tick
         *****************************************************/
        virtual void Tick(float fDelta) = 0;

        /*****************************************************
         * Draw
         *
         * Draws the current system
         *
         * @param fDelta Delta time between the last draw
         *****************************************************/
        virtual void Draw(float fDelta) = 0;
    };
}  // namespace IceSDK::Core::ECS
