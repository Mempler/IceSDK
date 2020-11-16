#pragma once

#include <entt/entt.hpp>

namespace IceSDK::Core::ECS
{
    class Entity
    {
    private:
        entt::entity m_eHandle = entt::null;

    public:
        Entity(const Memory::WeakPtr<entt::registry>& wpRegistry);
        Entity(const Memory::WeakPtr<entt::registry>& wpRegistry,
               entt::entity eHandle);
        ~Entity();

        template<typename T>
        bool HasComponent()
        {
            return this->_registry.lock()->has<T>(this->_inner);
        }

        template<typename T, typename... Args>
        T& AttachComponent(Args&&... pArgs)
        {
            const auto registry = this->_registry.lock();

            return registry->emplace<T>(this->_inner,
                                        std::forward<Args>(pArgs)...);
        }

        template<typename T>
        T& GetComponent()
        {
            const auto registry = this->_registry.lock();
            return registry->get<T>(this->_inner);
        }

        template<typename T>
        void DetachComponent()
        {
            const auto registry = this->_registry.lock();
            registry->remove_if_exists<T>(this->_inner);
        }
    };
}  // namespace IceSDK::Core::ECS
