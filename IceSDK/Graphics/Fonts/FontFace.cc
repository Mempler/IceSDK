#include "pch.h"

#include "Graphics/Fonts/FontFace.h"

#include "Utils/FileSystem.h"
#include "Utils/Instrumentor.h"
#include "Utils/Logger.h"

using namespace IceSDK;
using namespace IceSDK::Graphics;

static FT_Library g_FTLibrary;

Memory::Ptr<FontFace> FontFace::FromMemory(const std::vector<uint8_t>& pData,
                                           size_t pFontSize)
{
    ICESDK_PROFILE_FUNCTION();

    Memory::Ptr<FontFace> font = std::make_shared<FontFace>();
    FT_Error err;

    err = FT_New_Memory_Face(g_FTLibrary, pData.data(), pData.size(), 0,
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

Memory::Ptr<FontFace> FontFace::FromFile(const std::string& pPath,
                                         size_t pFontSize)
{
    ICESDK_PROFILE_FUNCTION();

    if (!FileSystem::Exists(pPath)) return nullptr;

    return FontFace::FromMemory(FileSystem::ReadBinaryFile(pPath), pFontSize);
}

void FontFace::Init()
{
    ICESDK_PROFILE_FUNCTION();

    FT_Error err;

    err = FT_Init_FreeType(&g_FTLibrary);
    if (err != FT_Err_Ok)
        ICESDK_CORE_CRITICAL("Failed to initialize FreeType! ({:x})", err);
}

void FontFace::SetSize(size_t size)
{
    ICESDK_PROFILE_FUNCTION();

    if (this->_size == size) return;
    FT_Error err;

    this->_size = size;
    err = FT_Set_Char_Size(this->_face, 0, this->_size << 6, 96, 96);
    if (err != FT_Err_Ok)
        ICESDK_CORE_ERROR("Failed to change Font size! ({:x})", err);

    _glyphCache.clear();
}

size_t FontFace::GetSize()
{
    ICESDK_PROFILE_FUNCTION();

    return this->_size;
}

Glyph& FontFace::GetGlyph(uint32_t pGlyph)
{
    ICESDK_PROFILE_FUNCTION();

    // We don't want a glyph to be loaded twice!
    if (this->_glyphCache.count(pGlyph)) return this->_glyphCache[pGlyph];

    FT_Load_Glyph(this->_face, pGlyph, FT_LOAD_DEFAULT);

    FT_GlyphSlot glyphSlot = this->_face->glyph;
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
