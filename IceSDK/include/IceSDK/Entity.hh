#pragma once

#include "IceSDK/Utils/Exception.hh"

#include <entt/entt.hpp>
#include <glm/vec3.hpp>
#include <spdlog/fmt/bundled/core.h>

#include <string_view>


#define ENTITY_EXCEPTION(_FMT, ...)                                            \
    ::IceSDK::EntityException(fmt::format(_FMT, __VA_ARGS__), __FILE__,        \
                              __LINE__)

namespace IceSDK
{
    class EntityException final : public Utils::BaseException
    {
    public:
        explicit EntityException(fmt::string_view svReason, const char* svWhere,
                                 size_t sLine) :
            Utils::BaseException("Entity", svReason, svWhere, sLine)
        {
        }
    };

    class Entity final
    {
    private:
        std::weak_ptr<entt::registry> m_wpRegistry;

        entt::entity m_eHandle = entt::null;

    public:
        explicit Entity(const std::weak_ptr<entt::registry>& wpRegistry) :
            m_wpRegistry(wpRegistry)
        {
            m_eHandle = wpRegistry.lock()->create();
        }

        explicit Entity(const std::weak_ptr<entt::registry>& wpRegistry,
                        entt::entity ehRaw) :
            m_wpRegistry(wpRegistry),
            m_eHandle(ehRaw)
        {
        }

        explicit Entity() { }

        /*****************************************************
         * IsValid
         *
         * Tests if an Entity is valid
         *
         * @param eEntity Given entity
         *
         * @return True if valid
         *****************************************************/
        static bool IsValid(const Entity& eEntity)
        {
            return eEntity.m_eHandle != entt::null;
        }

        /*****************************************************
         * HasComponent<T>
         *
         * Checks if this entity has a component
         *
         * @return True if has
         *****************************************************/
        template<typename T>
        bool HasComponent()
        {
            const auto registry = m_wpRegistry.lock();

            return registry->has<T>(m_eHandle);
        }

        /*****************************************************
         * GetComponent<T>
         *
         * Gets a specific component
         * Will fail if doesn't have
         *
         * @return True if has
         *****************************************************/
        template<typename T>
        T& GetComponent()
        {
            if (!this->HasComponent<T>())
                throw ENTITY_EXCEPTION(
                    "Can't get a component, Entity({}) is missing it!",
                    m_eHandle);

            const auto registry = m_wpRegistry.lock();

            return registry->get<T>(m_eHandle);
        }

        /*****************************************************
         * AttachComponent<T>
         *
         * Attach a component
         * It'll fail if the component has already
         * been attached
         *
         * @return Reference to the attached component
         *****************************************************/
        template<typename T, typename... Args>
        T& AttachComponent(Args&&... tArgs)
        {
            if (!this->HasComponent<T>())
                throw ENTITY_EXCEPTION(
                    "Can't attach a component, Entity({}) has it already!",
                    m_eHandle);

            const auto registry = m_wpRegistry.lock();

            return registry->emplace<T>(m_eHandle,
                                        std::forward<Args>(tArgs)...);
        }

        /*****************************************************
         * DetachComponent<T>
         *
         * Detaches a component, it won't fail if
         * it doesn't have it.
         *****************************************************/
        template<typename T>
        void DetachComponent()
        {
            const auto registry = m_wpRegistry.lock();

            registry->remove_if_exists<T>(m_eHandle);
        }

        /*****************************************************
         * WithPosition
         *
         * Sets the position of an Entity using the TransformComponent
         *
         * @param v3Position Target Position
         *
         * @see IceSDK::ECS::Components::TransformComponent
         *
         * @return self
         *****************************************************/
        Entity& WithPosition(const glm::vec3& v3Position);

        /*****************************************************
         * WithScale
         *
         * Sets the scale of an Entity using the TransformComponent
         *
         * @param v3Scale Target Scale
         *
         * @see IceSDK::ECS::Components::TransformComponent
         *
         * @return self
         *****************************************************/
        Entity& WithScale(const glm::vec3& v3Scale);

        /*****************************************************
         * WithRotation
         *
         * Sets the rotation of an Entity using the TransformComponent
         *
         * @param fDegrees Target Rotation in degrees
         *
         * @see IceSDK::ECS::Components::TransformComponent
         *
         * @return self
         *****************************************************/
        Entity& WithRotation(const float fDegrees);

        /*****************************************************
         * DefaultTransformation
         *
         * Attaches a default TransformComponent if doesn't exist
         *
         * @see IceSDK::ECS::Components::TransformComponent
         *
         * @return self
         *****************************************************/
        Entity& DefaultTransformation();

        /*
        // Sprite
        //  Comment:
        //      Attaches a SpriteComponent and any other required component
        //  Components:
        //      - TransformComponent
        //      - SpriteComponent
        //      - MeshComponent
        //      - ShaderComponent
        Entity& Sprite(Memory::Ptr<Graphics::Texture2D> pTexture, const
        glm::vec2& pSize = { -1.f, -1.f });

        // Text
        //  Comment:
        //      Attaches a TextComponent and any other required component to
        draw Text
        //  Components:
        //      - SpriteComponet
        //      - TransformComponent
        //      - SpriteComponent
        //      - MeshComponent
        //      - ShaderComponent
        Entity& Text(const std::string& pText, size_t pFontSize,
        Graphics::FontFaceHandle pFontFace);
        */
    };
}  // namespace IceSDK