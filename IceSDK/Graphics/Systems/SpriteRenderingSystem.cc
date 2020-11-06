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
#include "Graphics/Debug/Draw.h"
#include "Graphics/Shaders/compiled/fs_sprite.d3d11.h"
#include "Graphics/Shaders/compiled/fs_sprite.d3d12.h"
#include "Graphics/Shaders/compiled/fs_sprite.d3d9.h"
#include "Graphics/Shaders/compiled/fs_sprite.glsl.h"
#include "Graphics/Shaders/compiled/fs_sprite.metal.h"
#include "Graphics/Shaders/compiled/fs_sprite.vulkan.h"
#include "Graphics/Shaders/compiled/vs_sprite.d3d11.h"
#include "Graphics/Shaders/compiled/vs_sprite.d3d12.h"
#include "Graphics/Shaders/compiled/vs_sprite.d3d9.h"
#include "Graphics/Shaders/compiled/vs_sprite.glsl.h"
#include "Graphics/Shaders/compiled/vs_sprite.metal.h"
#include "Graphics/Shaders/compiled/vs_sprite.vulkan.h"

using namespace IceSDK;
using namespace IceSDK::Systems;
using namespace IceSDK::Components;
using namespace IceSDK::Graphics;
using namespace IceSDK::Graphics::Components;

void SpriteRenderingSystem::Tick(float pDelta)
{
    ICESDK_PROFILE_FUNCTION();
}

void SpriteRenderingSystem::Init()
{
    ICESDK_PROFILE_FUNCTION();

    Pos2DTexCoordColourVertex::Init();
    Graphics::FontFace::Init();

    auto shaderManager = GetGameBase()->GetShaderManager();

    shaderManager->AppendShader(
        "Sprite", bgfx::RendererType::Direct3D9, Shaders::eShaderType::Fragment,
        std::vector<uint8_t>(&fs_sprite_d3d9[0],
                             &fs_sprite_d3d9[sizeof fs_sprite_d3d9]));
    shaderManager->AppendShader(
        "Sprite", bgfx::RendererType::Direct3D11,
        Shaders::eShaderType::Fragment,
        std::vector<uint8_t>(&fs_sprite_d3d11[0],
                             &fs_sprite_d3d11[sizeof fs_sprite_d3d11]));
    shaderManager->AppendShader(
        "Sprite", bgfx::RendererType::Direct3D12,
        Shaders::eShaderType::Fragment,
        std::vector<uint8_t>(&fs_sprite_d3d12[0],
                             &fs_sprite_d3d12[sizeof fs_sprite_d3d12]));
    shaderManager->AppendShader(
        "Sprite", bgfx::RendererType::OpenGL, Shaders::eShaderType::Fragment,
        std::vector<uint8_t>(&fs_sprite_glsl[0],
                             &fs_sprite_glsl[sizeof fs_sprite_glsl]));
    shaderManager->AppendShader(
        "Sprite", bgfx::RendererType::Metal, Shaders::eShaderType::Fragment,
        std::vector<uint8_t>(&fs_sprite_metal[0],
                             &fs_sprite_metal[sizeof fs_sprite_metal]));
    shaderManager->AppendShader(
        "Sprite", bgfx::RendererType::Vulkan, Shaders::eShaderType::Fragment,
        std::vector<uint8_t>(&fs_sprite_vulkan[0],
                             &fs_sprite_vulkan[sizeof fs_sprite_vulkan]));

    shaderManager->AppendShader(
        "Sprite", bgfx::RendererType::Direct3D9, Shaders::eShaderType::Vertex,
        std::vector<uint8_t>(&vs_sprite_d3d9[0],
                             &vs_sprite_d3d9[sizeof vs_sprite_d3d9]));
    shaderManager->AppendShader(
        "Sprite", bgfx::RendererType::Direct3D11, Shaders::eShaderType::Vertex,
        std::vector<uint8_t>(&vs_sprite_d3d11[0],
                             &vs_sprite_d3d11[sizeof vs_sprite_d3d11]));
    shaderManager->AppendShader(
        "Sprite", bgfx::RendererType::Direct3D12, Shaders::eShaderType::Vertex,
        std::vector<uint8_t>(&vs_sprite_d3d12[0],
                             &vs_sprite_d3d12[sizeof vs_sprite_d3d12]));
    shaderManager->AppendShader(
        "Sprite", bgfx::RendererType::OpenGL, Shaders::eShaderType::Vertex,
        std::vector<uint8_t>(&vs_sprite_glsl[0],
                             &vs_sprite_glsl[sizeof vs_sprite_glsl]));
    shaderManager->AppendShader(
        "Sprite", bgfx::RendererType::Metal, Shaders::eShaderType::Vertex,
        std::vector<uint8_t>(&vs_sprite_metal[0],
                             &vs_sprite_metal[sizeof vs_sprite_metal]));
    shaderManager->AppendShader(
        "Sprite", bgfx::RendererType::Vulkan, Shaders::eShaderType::Vertex,
        std::vector<uint8_t>(&vs_sprite_vulkan[0],
                             &vs_sprite_vulkan[sizeof vs_sprite_vulkan]));
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