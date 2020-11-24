#pragma once

#include "IceSDK/Graphics/Texture2D.hh"

#include <glm/vec2.hpp>

#include <hb-ft.h>
#include <hb.hh>

#include <memory>
#include <string_view>
#include <unordered_map>
#include <vector>

#undef getenv  // Harfbuzz sets this but spdlog doesn't like it.

namespace IceSDK::Graphics
{
    struct Glyph
    {
        glm::vec2 m_fSize;  // Bottom && Right
        float m_fBearingBase;
        float m_fGlyphTop;

        std::vector<uint8_t> m_vPixelData;
    };

    class FontFace
    {
    private:
        hb_buffer_t* m_pBuffer;
        hb_font_t* m_pFont;
        FT_Face m_Face;
        size_t m_Size;

        std::unordered_map<uint32_t, Glyph> m_umGlyphCache;

    public:
        ~FontFace()
        {
            hb_buffer_destroy(this->m_pBuffer);
            hb_font_destroy(this->m_pFont);
        }

        static std::shared_ptr<FontFace> FromMemory(
            const std::vector<uint8_t>& vBuffer, size_t sFontSize);
        static std::shared_ptr<FontFace> FromFile(std::string_view sPath,
                                                  size_t sFontSize);

        static void Init();

        // NOTE: This will clear the whole cache
        // and all it's font atlasses! it'll slow down
        // on the first render
        void SetSize(size_t sSize);
        size_t GetSize();

        // Load a single character to a font atlas
        Glyph& GetGlyph(uint32_t uGlyph);

        hb_buffer_t* _hb_buffer() { return m_pBuffer; }

        hb_font_t* _hb_font() { return m_pFont; }
    };
}  // namespace IceSDK::Graphics
