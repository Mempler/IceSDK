#include "pch.h"

#include "SpriteBatch.h"

#include "Utils/Logger.h"

#include "GameBase.h"

using namespace IceSDK::Graphics;

SpriteBatch::SpriteBatch()
{
    this->_vertexBuffer = new VertexInfo[_maxVertices];
    uint32_t* quads = new uint32_t[_maxIndices];
    uint32_t offset = 0;

    for (uint32_t i = 0; i < _maxIndices; i += 6)
    {
        quads[i + 0] = offset + 0;
        quads[i + 1] = offset + 1;
        quads[i + 2] = offset + 2;

        quads[i + 3] = offset + 2;
        quads[i + 4] = offset + 3;
        quads[i + 5] = offset + 0;

        offset += 4;
    }

    this->_indexHandle = bgfx::createIndexBuffer(
        bgfx::copy(quads, _maxIndices * sizeof(uint32_t)), BGFX_BUFFER_INDEX32);
    delete[] quads;

    this->_vertexLayout.begin()
        .add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Float, true)
        .add(bgfx::Attrib::TexCoord1, 2, bgfx::AttribType::Float)
        .end();

    for (uint32_t i = 0; i < _maxTextureSlots; i++)
    {
        const std::string uni_name = "s_texColour" + std::to_string(i);
        this->_textureUniforms[i] =
            bgfx::createUniform(uni_name.c_str(), bgfx::UniformType::Sampler);
    }

    this->_vertexPositions[0] = { 1.0f, 1.0f, 0, 1 };
    this->_vertexPositions[1] = { 1.0f, 0.0f, 0, 1 };
    this->_vertexPositions[2] = { 0.0f, 0.0f, 0, 1 };
    this->_vertexPositions[3] = { 0.0f, 1.0f, 0, 1 };
}

SpriteBatch::~SpriteBatch() { }

void SpriteBatch::NewFrame()
{
    this->_indexes = 0;
    this->_vertexBufferPtr = this->_vertexBuffer;
    this->_textureIndex = 0;
}

void SpriteBatch::EndFrame()
{
    Flush();
}

void SpriteBatch::Flush()
{
    if (!this->_indexes) return;

    uint32_t data_size = (uint32_t)((uint8_t*) this->_vertexBufferPtr
                                    - (uint8_t*) this->_vertexBuffer);

    if (bgfx::getAvailTransientVertexBuffer(data_size, this->_vertexLayout))
    {
        bgfx::TransientVertexBuffer tvb;
        bgfx::allocTransientVertexBuffer(&tvb, data_size, this->_vertexLayout);
        memcpy(tvb.data, this->_vertexBuffer, data_size);
        bgfx::setVertexBuffer(0, &tvb);
    }

    uint32_t count = this->_indexes ? this->_indexes : _maxIndices;
    bgfx::setIndexBuffer(
        this->_indexHandle, 0,
        count);  // this is normal index buffer - bgfx::indexbufferhandle

    for (uint32_t i = 0; i < this->_textureIndex; i++)
    {
        bgfx::setTexture(i, this->_textureUniforms[i],
                         this->_textureSlots[i]->GetHandle());
    }

    bgfx::setState(0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A
                   | BGFX_STATE_BLEND_ALPHA);

    auto shader = GetGameBase()->GetShaderManager()->LoadProgram("Sprite");
    bgfx::submit(0, shader);
}

void SpriteBatch::FlushReset()
{
    EndFrame();

    this->_indexes = 0;
    this->_vertexBufferPtr = this->_vertexBuffer;
    this->_textureIndex = 0;
}

void SpriteBatch::SubmitTexturedQuad(Memory::Ptr<Texture2D> pTexture,
                                     const glm::vec2& pPosition,
                                     const glm::vec2& pSize,
                                     const glm::vec4& pColour)
{
    CheckIndexes();
    glm::mat4 transform =
        glm::translate(glm::mat4(1.0f), { pPosition.x, pPosition.y, 1 })
        * glm::scale(glm::mat4(1.0f), { pSize.x, pSize.y, 1.0f });

    bgfx::setTransform(glm::value_ptr(transform));

    std::array<glm::vec2, QUAD_COUNT> g_DefTexCoords = {
        glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 0.0f), glm::vec2(0.0f, 0.0f),
        glm::vec2(0.0f, 1.0f)
    };

    DrawIndexed(transform, this->_vertexPositions, g_DefTexCoords, pColour,
                SetTexture(pTexture));
}

void SpriteBatch::SubmitTiledSprite(Memory::Ptr<Texture2D> pTexture,
                                    const glm::vec2& pPosition,
                                    const glm::vec2& pSize,
                                    const glm::vec4& pTileInfo,
                                    const glm::vec4& pColour)
{
    CheckIndexes();
    glm::mat4 transform =
        glm::translate(glm::mat4(1.0f), { pPosition.x, pPosition.y, 1 })
        * glm::scale(glm::mat4(1.0f), { pSize.x, pSize.y, 1.0f });

    bgfx::setTransform(glm::value_ptr(transform));
    DrawIndexed(transform, this->_vertexPositions,
                MakeTiled(pTexture, pTileInfo), pColour, SetTexture(pTexture));
}

void SpriteBatch::CheckIndexes()
{
    if (this->_indexes >= _maxIndices) FlushReset();
}

void SpriteBatch::DrawIndexed(glm::mat4 pTransform,
                              glm::vec4 pVertexPosition[QUAD_COUNT],
                              std::array<glm::vec2, QUAD_COUNT> pUVs,
                              const glm::vec4& pColour, float pTextureID,
                              uint32_t pIndexCount)
{
    for (size_t i = 0; i < QUAD_COUNT; i++)
    {
        this->_vertexBufferPtr->pos = pTransform * pVertexPosition[i];
        this->_vertexBufferPtr->color = pColour;
        this->_vertexBufferPtr->texture_pos = pUVs[i];
        this->_vertexBufferPtr->batch_info = { pTextureID, 1 };
        //                                                 ^ this is tiling,
        //                                                 good for optimization
        this->_vertexBufferPtr++;
    }
    this->_indexes += pIndexCount;
}

float SpriteBatch::SetTexture(Memory::Ptr<Texture2D> pTexture)
{
    float textureIndex = 0.0f;
    for (uint32_t i = 0; i < this->_textureIndex; i++)
    {
        if (*this->_textureSlots[i] == *pTexture)
        {
            textureIndex = (float) i;
            break;
        }
    }

    if (textureIndex == 0.0f)
    {
        if (this->_textureIndex >= _maxTextureSlots) FlushReset();

        textureIndex = (float) this->_textureIndex;
        this->_textureSlots[this->_textureIndex] = pTexture;
        this->_textureIndex++;
    }

    return textureIndex;
}

std::array<glm::vec2, QUAD_COUNT> SpriteBatch::MakeTiled(
    Memory::Ptr<Texture2D> pTexture, const glm::vec4& pTileInfo)
{
    float X = (1.f / pTexture->Width()) * pTileInfo.x;
    float Y = 1.f - (1.f / pTexture->Height()) * pTileInfo.y;
    float W = (1.f / pTexture->Width()) * pTileInfo.z;
    float H = (1.f / pTexture->Height()) * pTileInfo.w;

    std::array<glm::vec2, QUAD_COUNT> ret;
    ret[0] = { X, Y };
    ret[1] = { X + W, Y };
    ret[2] = { X + W, Y - H };
    ret[3] = { X, Y - H };
    return ret;
}