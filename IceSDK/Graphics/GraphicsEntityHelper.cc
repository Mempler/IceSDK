#include "pch.h"

#include "ECS/Components/BaseComponent.h"
#include "ECS/Components/TransformComponent.h"
#include "ECS/Scene.h"

#include "Utils/Instrumentor.h"

#include "Graphics/Components/MeshComponent.h"
#include "Graphics/Components/ShaderComponent.h"
#include "Graphics/Components/SpriteComponent.h"
#include "Graphics/Components/TextComponent.h"
#include "Graphics/Debug/Draw.h"
#include "Graphics/EntityHelper.h"
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
#include "Graphics/Systems/SpriteRenderingSystem.h"
#include "Graphics/Systems/TextRenderingSystem.h"

using namespace IceSDK;
using namespace IceSDK::Graphics;
using namespace IceSDK::Graphics::Entity;

IceSDK::Entity Graphics::Entity::CreateSprite(
    Memory::Ptr<IceSDK::Scene> pScene,
    Memory::Ptr<Shaders::ShaderManager> pShaderManager,
    Memory::Ptr<Texture2D> pTex, const glm::vec3& position,
    const glm::vec2& size, float rotation)
{
    ICESDK_PROFILE_FUNCTION();

    auto entity = pScene->CreateEntity("Sprite");

    glm::vec2 TexSize{ 0 };
    if (size.x != -1.f && size.y != -1.f)
        TexSize = size;
    else if (pTex != nullptr)
        TexSize = { pTex->Width(), pTex->Height() };

    entity.AddComponent<IceSDK::Components::TransformComponent>(
        position, glm::vec3{ TexSize.x, TexSize.y, 1.0f }, rotation);

    entity.AddComponent<Graphics::Components::MeshComponent>(
        bgfx::createVertexBuffer(
            bgfx::makeRef(g_SpriteVertices, sizeof g_SpriteVertices),
            g_2DPosTexCoordColourLayout),
        bgfx::createIndexBuffer(
            bgfx::makeRef(g_SpriteIndices, sizeof g_SpriteIndices)));

    entity.AddComponent<Graphics::Components::ShaderComponent>(
        pShaderManager.get()->LoadProgram("Sprite"));

    entity.AddComponent<Graphics::Components::SpriteComponent>(TexSize, pTex);

    return entity;
}

IceSDK::Entity Graphics::Entity::CreateText(
    Memory::Ptr<IceSDK::Scene> pScene,
    Memory::Ptr<Shaders::ShaderManager> pShaderManager,
    const std::string& pText, size_t pFontSize, FontFaceHandle pFontFace)
{
    ICESDK_PROFILE_FUNCTION();

    auto entity = Entity::CreateSprite(pScene, pShaderManager, nullptr);

    auto& baseComponent =
        entity.GetComponent<IceSDK::Components::BaseComponent>();
    baseComponent.name = "TextSprite";

    entity.AddComponent<Graphics::Components::TextComponent>(
        pText, (uint64_t) 0, pFontSize, pFontFace);

    return entity;
}

void Graphics::Entity::Init(
    const Memory::Ptr<Graphics::Shaders::ShaderManager>& pShaderManager)
{
    ICESDK_PROFILE_FUNCTION();

    Pos2DTexCoordColourVertex::Init();
    Graphics::FontFace::Init();

    pShaderManager->AppendShader(
        "Sprite", bgfx::RendererType::Direct3D9, Shaders::eShaderType::Fragment,
        std::vector<uint8_t>(&fs_sprite_d3d9[0],
                             &fs_sprite_d3d9[sizeof fs_sprite_d3d9]));
    pShaderManager->AppendShader(
        "Sprite", bgfx::RendererType::Direct3D11,
        Shaders::eShaderType::Fragment,
        std::vector<uint8_t>(&fs_sprite_d3d11[0],
                             &fs_sprite_d3d11[sizeof fs_sprite_d3d11]));
    pShaderManager->AppendShader(
        "Sprite", bgfx::RendererType::Direct3D12,
        Shaders::eShaderType::Fragment,
        std::vector<uint8_t>(&fs_sprite_d3d12[0],
                             &fs_sprite_d3d12[sizeof fs_sprite_d3d12]));
    pShaderManager->AppendShader(
        "Sprite", bgfx::RendererType::OpenGL, Shaders::eShaderType::Fragment,
        std::vector<uint8_t>(&fs_sprite_glsl[0],
                             &fs_sprite_glsl[sizeof fs_sprite_glsl]));
    pShaderManager->AppendShader(
        "Sprite", bgfx::RendererType::Metal, Shaders::eShaderType::Fragment,
        std::vector<uint8_t>(&fs_sprite_metal[0],
                             &fs_sprite_metal[sizeof fs_sprite_metal]));
    pShaderManager->AppendShader(
        "Sprite", bgfx::RendererType::Vulkan, Shaders::eShaderType::Fragment,
        std::vector<uint8_t>(&fs_sprite_vulkan[0],
                             &fs_sprite_vulkan[sizeof fs_sprite_vulkan]));

    pShaderManager->AppendShader(
        "Sprite", bgfx::RendererType::Direct3D9, Shaders::eShaderType::Vertex,
        std::vector<uint8_t>(&vs_sprite_d3d9[0],
                             &vs_sprite_d3d9[sizeof vs_sprite_d3d9]));
    pShaderManager->AppendShader(
        "Sprite", bgfx::RendererType::Direct3D11, Shaders::eShaderType::Vertex,
        std::vector<uint8_t>(&vs_sprite_d3d11[0],
                             &vs_sprite_d3d11[sizeof vs_sprite_d3d11]));
    pShaderManager->AppendShader(
        "Sprite", bgfx::RendererType::Direct3D12, Shaders::eShaderType::Vertex,
        std::vector<uint8_t>(&vs_sprite_d3d12[0],
                             &vs_sprite_d3d12[sizeof vs_sprite_d3d12]));
    pShaderManager->AppendShader(
        "Sprite", bgfx::RendererType::OpenGL, Shaders::eShaderType::Vertex,
        std::vector<uint8_t>(&vs_sprite_glsl[0],
                             &vs_sprite_glsl[sizeof vs_sprite_glsl]));
    pShaderManager->AppendShader(
        "Sprite", bgfx::RendererType::Metal, Shaders::eShaderType::Vertex,
        std::vector<uint8_t>(&vs_sprite_metal[0],
                             &vs_sprite_metal[sizeof vs_sprite_metal]));
    pShaderManager->AppendShader(
        "Sprite", bgfx::RendererType::Vulkan, Shaders::eShaderType::Vertex,
        std::vector<uint8_t>(&vs_sprite_vulkan[0],
                             &vs_sprite_vulkan[sizeof vs_sprite_vulkan]));
}

void Graphics::Entity::InitScene(const Memory::Ptr<IceSDK::Scene>& pScene)
{
    ICESDK_PROFILE_FUNCTION();

    pScene->RegisterSystem<Systems::SpriteRenderingSystem>();
    pScene->RegisterSystem<Systems::TextRenderingSystem>();
}
