#pragma once

#include "IceSDK/Entity.hh"
#include "IceSDK/System.hh"

#include <entt/entt.hpp>

#include <string_view>

namespace IceSDK
{
    class Scene final
    {
    private:
        std::vector<System*> m_vpSystems;

        // This has to be a shared_ptr so we can later
        // pass it down to the systems
        std::shared_ptr<entt::registry> m_pRegistry;

    public:
        explicit Scene();
        ~Scene()
        {
            // Cleanup Systems
            for (auto&& system : m_vpSystems) { delete system; }
        }

        /*****************************************************
         * CreateEntity
         *
         * Creates an Entity which components can be attached
         * to. It'll be used to interact with the Engine
         * and non Engine systems
         *
         * @param svName Name of the Entity
         *
         * @return Entity
         *****************************************************/
        Entity CreateEntity(std::string_view svName) const;

        /*****************************************************
         * RegisterSystem
         *
         * Registers a system, later to be run within
         * the engine itself to modify the current
         * workflow
         *****************************************************/
        template<typename Sys>
        void RegisterSystem()
        {
            m_vpSystems.push_back(new Sys(GetRegistry()));
        }

        /*****************************************************
         * Tick
         *
         * Tick the underlying systems
         *
         * @param fDelta Delta time between the last tick
         *****************************************************/
        void Tick(float fDelta);

        /*****************************************************
         * Draw
         *
         * Draws the underlying systems
         *
         * @param fDelta Delta time between the last draw
         *****************************************************/
        void Draw(float fDelta);

        /*****************************************************
         * GetRegistry
         *
         * Gets the registry to interact with entities
         *
         * @return A weak ptr towards registry
         *         This might be null if Scene
         *         is no longer existent
         *****************************************************/
        std::weak_ptr<entt::registry> GetRegistry() { return m_pRegistry; }
    };
}  // namespace IceSDK
