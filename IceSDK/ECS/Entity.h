#pragma once

#include "Utils/Memory.h"

#include <entt/entt.hpp>

#include "Graphics/Texture2D.h"
#include "Graphics/Shaders/ShaderManager.h"
#include "Graphics/Fonts/FontManager.h"

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
            return this->_registry.lock()->has<T>(this->_inner);
        }

        template<typename T, typename... Args>
        T& AddComponent(Args&&... pArgs)
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
        void RemoveComponent()
        {
            const auto registry = this->_registry.lock();
            registry->remove_if_exists<T>(this->_inner);
        }

        // Position
        //  Comment:
        //      Sets the position of an Entity using the TransformComponent
        //  Components:
        //      - TransformComponent
        Entity& Position(glm::vec3 pPosition);
        // Scale
        //  Comment:
        //      Sets the scale of an Entity using the TransformComponent
        //  Components:
        //      - TransformComponent
        Entity& Scale(glm::vec3 pScale);
        // Rotation
        //  Comment:
        //      Sets the rotation of an Entity using the TransformComponent
        //  Components:
        //      - TransformComponent
        Entity& Rotation(float pDegrees);

        // DefaultTransformation
        //  Comment:
        //      Attaches a default TransformComponent if doesn't exist
        //  Components:
        //      - TransformComponent
        Entity& DefaultTransformation();

        // Sprite
        //  Comment:
        //      Attaches a SpriteComponent and any other required component
        //  Components:
        //      - TransformComponent
        //      - SpriteComponent
        //      - MeshComponent
        //      - ShaderComponent
        Entity& Sprite(Memory::Ptr<Graphics::Texture2D> pTexture, const glm::vec2& pSize = { -1.f, -1.f });

        // Text
        //  Comment:
        //      Attaches a TextComponent and any other required component to draw Text
        //  Components:
        //      - SpriteComponet
        //      - TransformComponent
        //      - SpriteComponent
        //      - MeshComponent
        //      - ShaderComponent
        Entity& Text(const std::string& pText, size_t pFontSize, Graphics::FontFaceHandle pFontFace);

        static bool IsValid(const Entity& pEntity);

    private:
        Memory::WeakPtr<entt::registry> _registry;
        entt::entity _inner = entt::null;
    };
}  // namespace IceSDK