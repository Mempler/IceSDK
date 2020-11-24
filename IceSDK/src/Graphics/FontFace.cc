#include "pch.h"

#include "Graphics/Fonts/FontFace.hh"

#include "Utils/FileSystem.hh"
#include "Utils/Logger.hh"

using namespace IceSDK;
using namespace IceSDK::Graphics;

static FT_Library g_FTLibrary;

std::shared_ptr<FontFace> FontFace::FromMemory(
    const std::vector<uint8_t>& vBuffer, size_t sFontSize)
{
    std::shared_ptr<FontFace> font = std::make_shared<FontFace>();
    FT_Error err;

    err = FT_New_Memory_Face(g_FTLibrary, vData.data(), vData.size(), 0,
                             &font->_face);
    if (err != FT_Err_Ok)
        ICESDK_CORE_ERROR("Failed to load FontFace! ({:x})", err);

    font->_font = hb_ft_font_create(font->_face, NULL);
    font->_buffer = hb_buffer_create();

    if (!hb_buffer_allocation_successful(font->_buffer))
        ICESDK_CORE_ERROR("Failed to allocate HarfBuzz buffer!");

    for (int i = 0; i < font->_face->num_charmaps;
         i++)  // Forcefully load UTF-16 characters
    {
        if (((font->_face->charmaps[i]->platform_id == 0)
             && (font->_face->charmaps[i]->encoding_id == 3))
            || ((font->_face->charmaps[i]->platform_id == 3)
                && (font->_face->charmaps[i]->encoding_id == 1)))
        {
            err = FT_Set_Charmap(font->_face, font->_face->charmaps[i]);

            if (err != FT_Err_Ok)
                ICESDK_CORE_ERROR("Failed to set charmap! ({:x})", err);
        }
    }

    // Cache most important ascii characters, everything else can be cached at
    // runtime
    const char* ascii =
        " abcdefghijklmnopqrstuvwxyABCDEFGHIJKLMNOPQRSTUVWXYZ123456789!?.,";

    font->SetSize(pFontSize);

    for (size_t i = 0; i < 65; i++)
    {
        FT_Error err;
        err = FT_Load_Char(
            font->_face, ascii[i],
            FT_LOAD_RENDER | FT_LOAD_NO_HINTING | FT_LOAD_TARGET_LIGHT);
        if (err != FT_Err_Ok)
            ICESDK_CORE_ERROR("Failed to load Font Glyph! ({:x})", err);

        font->GetGlyph(font->_face->glyph->glyph_index);
    }

    return font;
}

std::shared_ptr<FontFace> FontFace::FromFile(const std::string& pPath,
                                             size_t pFontSize)
{
    if (!FileSystem::Exists(pPath)) return nullptr;

    return FontFace::FromMemory(FileSystem::ReadBinaryFile(pPath), pFontSize);
}

void FontFace::Init()
{
    FT_Error err;

    err = FT_Init_FreeType(&g_FTLibrary);
    if (err != FT_Err_Ok)
        ICESDK_CORE_CRITICAL("Failed to initialize FreeType! ({:x})", err);
}

void FontFace::SetSize(size_t size)
{
    if (this->m_pSize == size) return;
    FT_Error err;

    this->m_pSize = size;
    err = FT_Set_Char_Size(this->m_pFace, 0, this->m_pSize << 6, 96, 96);
    if (err != FT_Err_Ok)
        ICESDK_CORE_ERROR("Failed to change Font size! ({:x})", err);

    _glyphCache.clear();
}

size_t FontFace::GetSize()
{
    return this->m_pSize;
}

Glyph& FontFace::GetGlyph(uint32_t pGlyph)
{
    // We don't want a glyph to be loaded twice!
    if (this->_glyphCache.count(pGlyph)) return this->_glyphCache[pGlyph];

    FT_Load_Glyph(this->m_pFace, pGlyph, FT_LOAD_DEFAULT);

    FT_GlyphSlot glyphSlot = this->m_pFace->glyph;
    FT_Render_Glyph(glyphSlot, FT_RENDER_MODE_NORMAL);

    FT_Bitmap* bmp = &glyphSlot->bitmap;
    std::vector<uint8_t> pixel_data;
    pixel_data.reserve(bmp->width * bmp->rows * 4);

    for (size_t row = 0; row < bmp->rows; ++row)
    {
        for (size_t col = 0; col < bmp->width; ++col)
        {
            auto pixel = (uint8_t) bmp->buffer[row * bmp->pitch + col];

            pixel_data.push_back(pixel);
            pixel_data.push_back(pixel);
            pixel_data.push_back(pixel);

            if (pixel > 0)
                pixel_data.push_back(255);
            else
                pixel_data.push_back(0);
        }
    }

    Glyph glyph{ { bmp->width, bmp->rows },
                 (float) (glyphSlot->metrics.horiBearingY >> 6),
                 (float) (glyphSlot->bitmap_top),
                 pixel_data };

    this->_glyphCache.insert({ pGlyph, glyph });

    return this->_glyphCache[pGlyph];
}
