#pragma once

#include "ECS/Entity.h"
#include "ECS/System.h"

#include <entt/entt.hpp>

#include <mutex>

namespace IceSDK
{
    class Scene final
    {
    public:
        explicit Scene();

        Entity CreateEntity(const std::string& pName) const;

        template<typename Sys>
        void RegisterSystem()
        {
            ICESDK_PROFILE_FUNCTION();

            auto registry = Memory::WeakPtr<entt::registry>(this->_registry);

            std::lock_guard<std::mutex> systemLock(_mut_systems);
            _systems.push_back(std::make_shared<Sys>(registry));
        }

        Entity CreateCamera() const;

        void Tick(float pDelta);
        void Draw(float pDelta);

        Memory::WeakPtr<entt::registry> GetRegistry();

    private:
        std::mutex _mut_systems;
        std::vector<Memory::Ptr<System>> _systems;

        Memory::Ptr<entt::registry> _registry;
    };
}  // namespace IceSDK
