#include "pch.h"

#include "Graphics/Fonts/FontManager.h"

#include "Utils/FileSystem.h"
#include "Utils/Logger.h"

using namespace IceSDK;
using namespace IceSDK::Graphics;

std::tuple<Glyph, Memory::Ptr<FontFace>> FontManager::GetGlyph(
    FontFaceHandle pFont, uint32_t pGlyph, size_t pSize)
{
    if (pFont == INVALID_FONT_FACE_HANDLE)
        ICESDK_CORE_CRITICAL(
            "Passed FontFaceHandle is invalid! f: {} g: {} s: {}", pFont,
            pGlyph, pSize);

    auto fontFaces = this->_faces[pFont];

    for (auto&& face : fontFaces)
    {
        if (face->GetSize() == pSize)
        {
            auto glyph = face->GetGlyph(pGlyph);

            return { glyph, face };
        }
    }

    auto face = FontFace::FromMemory(this->_faces_buffer[pFont], pSize);
    auto fontBuffer = this->_faces[pFont];
    fontBuffer.push_back(face);

    auto glyph = face->GetGlyph(pGlyph);

    return { glyph, face };
}

FontFaceHandle FontManager::LoadFont(const std::string& pPath)
{
    if (!FileSystem::Exists(pPath)) return INVALID_FONT_FACE_HANDLE;

    return FontManager::LoadFont(FileSystem::ReadBinaryFile(pPath));
}

FontFaceHandle FontManager::LoadFont(std::vector<uint8_t> pBuffer)
{
    auto fontHandle = this->_face_index++;
    auto font = FontFace::FromMemory(pBuffer, 12);  // 12 is the default face.

    if (font == nullptr) return INVALID_FONT_FACE_HANDLE;

    std::vector<Memory::Ptr<FontFace>> fontBuffer;
    fontBuffer.push_back(font);
    this->_faces.insert({ fontHandle, fontBuffer });
    this->_faces_buffer.insert({ fontHandle, pBuffer });

    return fontHandle;
}
