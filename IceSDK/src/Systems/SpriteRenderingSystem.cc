#include "pch.h"

#include "ECS/Systems/SpriteRenderingSystem.hh"

#include "ECS/Components/CameraComponent.hh"
#include "ECS/Components/MeshComponent.hh"
#include "ECS/Components/ShaderComponent.hh"
#include "ECS/Components/SpriteComponent.hh"
#include "ECS/Components/TransformComponent.hh"
#include "ECS/Entity.hh"
#include "ECS/Systems/CameraSystem.hh"

#include "GameBase.hh"
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

#include <vector>

using namespace IceSDK;
using namespace IceSDK::ECS::Systems;
using namespace IceSDK::ECS::Components;
using namespace IceSDK::Graphics;

void SpriteRenderingSystem::Tick(float fDelta) { }

void SpriteRenderingSystem::Init()
{
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

void SpriteRenderingSystem::Draw(float fDelta)
{
    const auto registry = m_wpRegistry.lock();

    auto spriteGroup = registry->view<SpriteComponent>();
    for (auto rawSpriteEntity : spriteGroup)
    {
        auto spriteEntity = Entity(m_wpRegistry, rawSpriteEntity);

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