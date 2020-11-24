// Debug helper for drawing blank squares onto the screen without any entity
// nor any system

#pragma once

#include "IceSDK/GameBase.hh"
#include "IceSDK/Graphics/Texture2D.hh"

#include <bgfx/bgfx.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>

#include <cstdint>
#include <vector>

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
    inline void DrawQuad(glm::vec2 v2Position, glm::vec2 v2Size,
                         glm::vec4 v4Colour)
    {
        auto Model = glm::mat4{ 1 };
        auto View = glm::mat4{ 1 };
        auto Projection = glm::mat4{ 1 };

        const auto window = GetGameBase()->GetGameWindow();
        Projection = glm::ortho(0.0f, static_cast<float>(window->Width()),
                                static_cast<float>(window->Height()), 0.0f,
                                0.1f, 1000.0f);
        Projection[3].z = 1;

        bgfx::setViewTransform(128, glm::value_ptr(View),
                               glm::value_ptr(Projection));

        auto tex = Texture2D::Create("dbgBox", v2Size.x, v2Size.y,
                                     bgfx::TextureFormat::RGBA8);

        std::vector<uint8_t> pixelData;
        pixelData.reserve(v2Size.x * v2Size.y * 4);
        for (size_t i = 0; i < v2Size.x * v2Size.y; i++)
        {
            pixelData.push_back(v4Colour.r);
            pixelData.push_back(v4Colour.g);
            pixelData.push_back(v4Colour.b);
            pixelData.push_back(v4Colour.a);
        }

        tex->Modify({ { 0.0, 0.0 }, v2Size }, pixelData,
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
