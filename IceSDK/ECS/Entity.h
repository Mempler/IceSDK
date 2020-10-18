#pragma once

#include "Utils/Instrumentor.h"
#include "Utils/Memory.h"

#include <entt/entt.hpp>

namespace IceSDK
{
    class Entity final
    {
    public:
        explicit Entity(const Memory::WeakPtr<entt::registry>& pRegistry);
        explicit Entity(const Memory::WeakPtr<entt::registry>& pRegistry,
                        entt::entity pEntity);
        explicit Entity();

        template<typename T>
        bool HasComponent()
        {
            ICESDK_PROFILE_FUNCTION();

            return this->_registry.lock()->has<T>(this->_inner);
        }

        template<typename T, typename... Args>
        T& AddComponent(Args&&... pArgs)
        {
            ICESDK_PROFILE_FUNCTION();

            const auto registry = this->_registry.lock();

            return registry->emplace<T>(this->_inner,
                                        std::forward<Args>(pArgs)...);
        }

        template<typename T>
        T& GetComponent()
        {
            ICESDK_PROFILE_FUNCTION();

            const auto registry = this->_registry.lock();
            return registry->get<T>(this->_inner);
        }

        template<typename T>
        bool RemoveComponent(T pComponent)
        {
            ICESDK_PROFILE_FUNCTION();

            if (!this->HasComponent(pComponent)) return false;

            const auto registry = this->_registry.lock();
            registry->remove_all(this->_inner, pComponent);

            return true;
        }

        static bool IsValid(const Entity& pEntity);

    private:
        Memory::WeakPtr<entt::registry> _registry;
        entt::entity _inner = entt::null;
    };
}  // namespace IceSDK
