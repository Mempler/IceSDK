#include "pch.h"

#include "Graphics/Systems/SpriteRenderingSystem.h"

#include "ECS/Components/CameraComponent.h"
#include "ECS/Components/TransformComponent.h"
#include "ECS/Entity.h"
#include "ECS/Systems/CameraSystem.h"

#include "Utils/Instrumentor.h"

#include "GameBase.h"
#include "Graphics/Components/MeshComponent.h"
#include "Graphics/Components/ShaderComponent.h"
#include "Graphics/Components/SpriteComponent.h"

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

    auto spriteGroup = registry->view<SpriteComponent>();
    for (auto rawSpriteEntity : spriteGroup)
    {
        auto spriteEntity = Entity(this->_registry, rawSpriteEntity);

        auto& mesh = spriteEntity.GetComponent<MeshComponent>();
        auto& transform = spriteEntity.GetComponent<TransformComponent>();
        auto& sprite = spriteEntity.GetComponent<SpriteComponent>();
        auto& shader = spriteEntity.GetComponent<ShaderComponent>();

        if (sprite.texture == nullptr || !bgfx::isValid(mesh.index_buffer)
            || !bgfx::isValid(mesh.vertex_buffer)
            || !bgfx::isValid(sprite.texture->GetHandle())
            || !bgfx::isValid(shader.handle))
            continue;

        GetGameBase()->GetSpriteBatch()->SubmitTexturedQuad(
            sprite.texture, transform.position, sprite.size, { 1, 1, 1, 1 });
    }
}
