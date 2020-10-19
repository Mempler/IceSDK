#include "pch.h"

#include "Graphics/Systems/SpriteRenderingSystem.h"

#include "ECS/Components/CameraComponent.h"
#include "ECS/Components/TransformComponent.h"
#include "ECS/Entity.h"
#include "ECS/Systems/CameraSystem.h"

#include "Utils/Instrumentor.h"

#include "GameBase.h"
#include "Graphics/Components/ShaderComponent.h"
#include "Graphics/Components/SpriteComponent.h"
#include "Graphics/Components/TileComponent.h"

using namespace IceSDK;
using namespace IceSDK::Systems;
using namespace IceSDK::Components;
using namespace IceSDK::Graphics;
using namespace IceSDK::Graphics::Components;

void SpriteRenderingSystem::Tick(float pDelta)
{
    ICESDK_PROFILE_FUNCTION();
}

void SpriteRenderingSystem::Draw(float pDelta)
{
    ICESDK_PROFILE_FUNCTION();

    const auto registry = this->_registry.lock();
    if (registry == nullptr) return;

    auto spriteGroup =
        registry->group<SpriteComponent>(entt::get<TransformComponent>);
    for (auto rawSpriteEntity : spriteGroup)
    {
        ICESDK_PROFILE_SCOPE("Sprite Render");

        auto spriteEntity = Entity(this->_registry, rawSpriteEntity);

        auto& sprite = spriteEntity.GetComponent<SpriteComponent>();
        if (sprite.texture == nullptr
            || !bgfx::isValid(sprite.texture->GetHandle()))
            continue;

        auto& transform = spriteEntity.GetComponent<TransformComponent>();
        if (spriteEntity.HasComponent<TileComponent>())
        {
            auto& tile = spriteEntity.GetComponent<TileComponent>();
            GetGameBase()->GetSpriteBatch()->SubmitTiledSprite(
                sprite.texture, transform.position, sprite.size, tile.info,
                { 1, 1, 1, 1 });
        }
        else
            GetGameBase()->GetSpriteBatch()->SubmitTexturedQuad(
                sprite.texture, transform.position, sprite.size,
                { 1, 1, 1, 1 });
    }
}
