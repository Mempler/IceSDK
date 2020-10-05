#include "pch.h"

#include "Graphics/Systems/SpriteRenderingSystem.h"

#include "ECS/Components/CameraComponent.h"
#include "ECS/Components/TransformComponent.h"
#include "ECS/Entity.h"
#include "ECS/Systems/CameraSystem.h"

#include "Utils/Instrumentor.h"

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

        const auto u_tex_colour =
            bgfx::createUniform("s_texColour", bgfx::UniformType::Sampler);

        bgfx::setTransform(glm::value_ptr(transform.model_matrix));

        bgfx::setVertexBuffer(0, mesh.vertex_buffer);
        bgfx::setIndexBuffer(mesh.index_buffer);
        bgfx::setTexture(0, u_tex_colour, sprite.texture->GetHandle());

        bgfx::setState(0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A
                       | BGFX_STATE_BLEND_ALPHA);

        bgfx::submit(0, shader.handle);
    }
}
