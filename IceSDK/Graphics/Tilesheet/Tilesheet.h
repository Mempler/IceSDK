#pragma once

#include "Utils/Memory.h"

#include "Graphics/Texture2D.h"

#include <glm/glm.hpp>

#include <vector>

namespace IceSDK::Graphics
{
    class Tilesheet
    {
    public:
        explicit Tilesheet(Memory::Ptr<Texture2D> pTexture, int pSize);
        ~Tilesheet();

        glm::vec4 GetTile(int pID);
        Memory::Ptr<Texture2D> GetTexture();
        uint32_t TileSize();

    private:
        Memory::Ptr<Texture2D> _texture;
        std::vector<glm::vec4> _textureCoordinates;
    };
}  // namespace IceSDK::Graphics