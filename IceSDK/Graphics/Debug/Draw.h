// Debug helper for drawing blank squares onto the screen without any entity
// nor any system

#pragma once

#include "GameBase.h"
#include "Graphics/EntityHelper.h"
#include "Graphics/Texture2D.h"

#include <bgfx/bgfx.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>

static bgfx::VertexLayout g_2DPosTexCoordColourLayout;

struct Pos2DTexCoordColourVertex
{
    glm::vec2 pos;
    glm::vec2 uv;
    glm::vec4 colour;

    static void Init()
    {
        g_2DPosTexCoordColourLayout.begin()
            .add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Float)
            .end();
    }
};

static Pos2DTexCoordColourVertex g_SpriteVertices[4] = {
    { { 1.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
    { { 1.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
    { { 0.0f, 0.0f }, { 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
    { { 0.0f, 1.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
};

static const uint16_t g_SpriteIndices[6] = {
    0, 1, 3, 1, 2, 3,
};

namespace IceSDK::Graphics::Debug
{
    // NOTE: this should be called in a draw loop
    inline void DrawQuad(glm::vec2 pos, glm::vec2 size, glm::vec4 colour)
    {
        auto Model = glm::mat4{ 1 };
        auto View = glm::mat4{ 1 };
        auto Projection = glm::mat4{ 1 };

        const auto window = GetWindow();
        Projection = glm::ortho(0.0f, static_cast<float>(window->Width()),
                                static_cast<float>(window->Height()), 0.0f,
                                0.1f, 1000.0f);
        Projection[3].z = 1;

        bgfx::setViewTransform(128, glm::value_ptr(View),
                               glm::value_ptr(Projection));

        auto tex = Texture2D::Create("dbgBox", size.x, size.y,
                                     bgfx::TextureFormat::RGBA8);

        std::vector<uint8_t> pixelData;
        pixelData.reserve(size.x * size.y * 4);
        for (size_t i = 0; i < size.x * size.y; i++)
        {
            pixelData.push_back(colour.r);
            pixelData.push_back(colour.g);
            pixelData.push_back(colour.b);
            pixelData.push_back(colour.a);
        }

        tex->Modify({ { 0.0, 0.0 }, size }, pixelData,
                    bgfx::TextureFormat::RGBA8);

        auto vertexBuffer = bgfx::createVertexBuffer(
            bgfx::makeRef(g_SpriteVertices, sizeof g_SpriteVertices),
            g_2DPosTexCoordColourLayout);

        auto indexBuffer = bgfx::createIndexBuffer(
            bgfx::makeRef(g_SpriteIndices, sizeof g_SpriteIndices));

        const auto u_tex_colour =
            bgfx::createUniform("s_texColour", bgfx::UniformType::Sampler);

        bgfx::setTransform(glm::value_ptr(Model));

        bgfx::setVertexBuffer(0, vertexBuffer);
        bgfx::setIndexBuffer(indexBuffer);
        bgfx::setTexture(0, u_tex_colour, tex->GetHandle());

        bgfx::setState(0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A
                       | BGFX_STATE_BLEND_ALPHA);

        auto shader = GetGameBase()->GetShaderManager()->LoadProgram("Sprite");

        bgfx::submit(128, shader);

        bgfx::destroy(vertexBuffer);
        bgfx::destroy(indexBuffer);
    }

}  // namespace IceSDK::Graphics::Debug
