#include "pch.hh"

#include "Entity.hh"

#include "ECS/Components/TransformComponent.hh"
#include "ECS/Scene.hh"

#include "GameBase.hh"
/*
#include "Graphics/Components/MeshComponent.h"
#include "Graphics/Components/ShaderComponent.h"
#include "Graphics/Components/SpriteComponent.h"
#include "Graphics/Components/TextComponent.h"
*/

using namespace IceSDK::ECS;

Entity& Entity::WithPosition(const glm::vec3& v3Position)
{
    DefaultTransformation();  // Setup default transformation

    auto& transformComponent = GetComponent<Components::TransformComponent>();

    transformComponent.m_v3Position = v3Position;

    return *this;
}

Entity& Entity::WithScale(const glm::vec3& v3Scale)
{
    DefaultTransformation();  // Setup default transformation

    auto& transformComponent = GetComponent<Components::TransformComponent>();

    transformComponent.m_v3Scale = v3Scale;

    return *this;
}

Entity& Entity::WithRotation(const float fDegrees)
{
    DefaultTransformation();  // Setup default transformation

    auto& transformComponent = GetComponent<Components::TransformComponent>();

    transformComponent.m_fRotation = fDegrees;

    return *this;
}

Entity& Entity::DefaultTransformation()
{
    if (HasComponent<Components::TransformComponent>()) return *this;

    auto& transformComponent =
        AttachComponent<Components::TransformComponent>();

    transformComponent.m_fRotation = 0.f;
    transformComponent.m_v3Position = { 0.f, 0.f, 0.f };
    transformComponent.m_v3Scale = { 1.f, 1.f, 1.f };

    return *this;
}
/*
Entity& Entity::DefaultTransformation()
{
    if (this->HasComponent<Components::TransformComponent>()) return *this;

    auto& transformComponent =
        this->AddComponent<Components::TransformComponent>();

    transformComponent.rotation = 0.f;
    transformComponent.position = { 0.f, 0.f, 0.f };
    transformComponent.scale = { 1.f, 1.f, 1.f };

    return *this;
}

Entity& Entity::Sprite(Memory::Ptr<Graphics::Texture2D> pTexture,
                       const glm::vec2& pSize)
{
    if (this->HasComponent<Graphics::Components::SpriteComponent>())
        return *this;

    if (!this->HasComponent<Components::TransformComponent>())
    {
        auto& transformComponent =
            this->AddComponent<Components::TransformComponent>();
        transformComponent.scale = { 1.f, 1.f, 1.f };
    }

    this->AddComponent<Graphics::Components::MeshComponent>(
        bgfx::createVertexBuffer(
            bgfx::makeRef(g_SpriteVertices, sizeof g_SpriteVertices),
            g_2DPosTexCoordColourLayout),
        bgfx::createIndexBuffer(
            bgfx::makeRef(g_SpriteIndices, sizeof g_SpriteIndices)));

    this->AddComponent<Graphics::Components::ShaderComponent>(
        GetGameBase()->GetShaderManager()->LoadProgram("Sprite"));

    glm::vec2 texSize{ 0 };
    if (pSize.x != -1.f && pSize.y != -1.f)
        texSize = pSize;
    else if (pTexture != nullptr)
        texSize = { pTexture->Width(), pTexture->Height() };

    this->AddComponent<Graphics::Components::SpriteComponent>(texSize,
                                                              pTexture);

    return *this;
}

Entity& Entity::Text(const std::string& pText, size_t pFontSize,
                     Graphics::FontFaceHandle pFontFace)
{
    // Make sure any prior text components get removed
    this->RemoveComponent<Graphics::Components::TextComponent>();

    this->Sprite(nullptr);

    this->AddComponent<Graphics::Components::TextComponent>(
        pText, (uint64_t) 0, pFontSize, pFontFace);

    return *this;
}
*/