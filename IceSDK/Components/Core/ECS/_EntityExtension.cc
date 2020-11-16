#include "pch.h"

#include "ECS/Components/TransformComponent.h"
#include "ECS/Scene.h"
#include "Entity.h"

#include "Utils/Instrumentor.h"

#include "GameBase.h"
#include "Graphics/Components/MeshComponent.h"
#include "Graphics/Components/ShaderComponent.h"
#include "Graphics/Components/SpriteComponent.h"
#include "Graphics/Components/TextComponent.h"
#include "Graphics/Debug/Draw.h"

using namespace IceSDK;

Entity& Entity::Position(glm::vec3 pPosition)
{
    ICESDK_PROFILE_FUNCTION();

    this->DefaultTransformation();

    auto& transformComponent =
        this->GetComponent<Components::TransformComponent>();

    transformComponent.position = pPosition;

    return *this;
}

Entity& Entity::Scale(glm::vec3 pScale)
{
    ICESDK_PROFILE_FUNCTION();

    this->DefaultTransformation();

    auto& transformComponent =
        this->GetComponent<Components::TransformComponent>();

    transformComponent.scale = pScale;

    return *this;
}

Entity& Entity::Rotation(float pRotation)
{
    ICESDK_PROFILE_FUNCTION();

    this->DefaultTransformation();

    auto& transformComponent =
        this->GetComponent<Components::TransformComponent>();

    transformComponent.rotation = pRotation;

    return *this;
}

Entity& Entity::DefaultTransformation()
{
    ICESDK_PROFILE_FUNCTION();

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
    ICESDK_PROFILE_FUNCTION();

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
    ICESDK_PROFILE_FUNCTION();

    // Make sure any prior text components get removed
    this->RemoveComponent<Graphics::Components::TextComponent>();

    this->Sprite(nullptr);

    this->AddComponent<Graphics::Components::TextComponent>(
        pText, (uint64_t) 0, pFontSize, pFontFace);

    return *this;
}
