#pragma once

#include "IceSDK/Graphics/Texture2D.hh"

#include <bgfx/bgfx.h>
#include <glm/glm.hpp>

#include <array>

#define QUAD_COUNT 4

namespace IceSDK::Graphics
{
    static const uint32_t g_MaxQuads = 20000;
    static const uint32_t g_MaxVertices = g_MaxQuads * 4;
    static const uint32_t g_MaxIndices = g_MaxQuads * 6;
    static const uint32_t g_MaxTextureSlots = 16;

    struct VertexInfo
    {
        glm::vec2 m_v2Pos;
        glm::vec2 m_v2TexturePos;
        glm::vec4 m_v4Color;
        glm::vec2 m_v2BatchInfo;
    };

    class SpriteBatch
    {
    public:
        explicit SpriteBatch();
        ~SpriteBatch();

        void NewFrame();
        void EndFrame();
        void Flush();
        void FlushReset();

        void SubmitTexturedQuad(const Texture2D& hTexture,
                                const glm::vec2& v2Position,
                                const glm::vec2& v2Size,
                                const glm::vec4& v4Colour);

        void SubmitTiledSprite(const Texture2D& hTexture,
                               const glm::vec2& v2Position,
                               const glm::vec2& v2Size,
                               const glm::vec4& v4TileInfo,
                               const glm::vec4& v4Colour);

        void CheckIndexes();
        void DrawIndexed(glm::mat4 m4Transform,
                         glm::vec4 v4VertexPosition[QUAD_COUNT],
                         std::array<glm::vec2, QUAD_COUNT> v2UVs,
                         const glm::vec4& v4Colour, float fTextureID = 0.f,
                         uint32_t uIndexCount = 6);

        float SetTexture(const Texture2D& hTexture);

        std::array<glm::vec2, QUAD_COUNT> MakeTiled(
            const Texture2D& hTexture, const glm::vec4& v4TileInfo);

    private:
        /*
            Sprite batch,
            some raw pointers don't require smart pointers, let
            them stay raw. we already delete them at the end so
            don't bother adding any smart pointer for quads or
            vertex_XX
        */
        uint32_t m_uIndexes;
        uint32_t m_uTextureIndex;

        glm::vec4 m_v4VertexPositions[4];
        std::array<Texture2D, g_MaxTextureSlots> m_hTextureSlots;
        //                                 ^^^^^^^^^^^^^^^^^ texture count -
        //                                 we can get it with bgfx::caps

        VertexInfo* m_pVertexBuffer;
        VertexInfo* m_pVertexBufferPtr;

        bgfx::VertexLayout m_VertexLayout;
        bgfx::IndexBufferHandle m_hIndexHandle;

        std::array<bgfx::UniformHandle, g_MaxTextureSlots>
            m_hTextureUniforms;  // size of that member must be same as
                                 // _textureSlots
    };

}  // namespace IceSDK::Graphics
