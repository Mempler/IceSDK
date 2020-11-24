#pragma once

#include "IceSDK/Graphics/FontFace.hh"

#include <tuple>
#include <unordered_map>
#include <vector>

#define INVALID_FONT_FACE_HANDLE -1

namespace IceSDK::Graphics
{
    typedef int32_t FontFaceHandle;

    class FontManager
    {
    public:
        std::tuple<Glyph, FontFace*> GetGlyph(FontFaceHandle hFont,
                                              uint32_t uGlyph, size_t sSize);

        FontFaceHandle LoadFont(std::vector<uint8_t> pBuffer);
        FontFaceHandle LoadFont(const std::string& pPath);

    private:
        FontFaceHandle m_hFaceIndex;
        std::unordered_map<FontFaceHandle, std::vector<uint8_t>>
            m_umFacesBuffer;
        std::unordered_map<FontFaceHandle, std::vector<FontFace*>> m_umFaces;
    };
}  // namespace IceSDK::Graphics
