/*
 * Copyright 2011-2020 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

#ifndef IMGUI_H_HEADER_GUARD
#define IMGUI_H_HEADER_GUARD

#include <bgfx/bgfx.h>

#include <imgui.h>

#define IMGUI_MBUT_LEFT 0x01
#define IMGUI_MBUT_RIGHT 0x02
#define IMGUI_MBUT_MIDDLE 0x04

inline uint32_t imguiRGBA(uint8_t uR, uint8_t rG, uint8_t rB, uint8_t rA = 255)
{
    return 0 | (uint32_t(uR) << 0) | (uint32_t(rG) << 8) | (uint32_t(rB) << 16)
           | (uint32_t(rA) << 24);
}

namespace bx
{
    struct AllocatorI;
}

void imguiCreate(float fFontSize = 18.0f, bx::AllocatorI* pAllocator = NULL);
void imguiDestroy();

void imguiBeginFrame(int32_t iMx, int32_t iMy, uint8_t uButton, int32_t iScroll,
                     uint16_t uWidth, uint16_t uHeight, int32_t iInputChar = -1,
                     bgfx::ViewId hView = 255);
void imguiEndFrame();

void imguiUpdateSize(int32_t iFontSize, int32_t iWidth, int32_t iHeight);

namespace ImGui
{
#define IMGUI_FLAGS_NONE UINT8_C(0x00)
#define IMGUI_FLAGS_ALPHA_BLEND UINT8_C(0x01)

    inline ImTextureID toId(bgfx::TextureHandle hTexture, uint8_t uFlags,
                            uint8_t uMip)
    {
        union
        {
            struct
            {
                bgfx::TextureHandle handle;
                uint8_t flags;
                uint8_t mip;
            } s;
            ImTextureID id;
        } tex;
        tex.s.handle = hTexture;
        tex.s.flags = uFlags;
        tex.s.mip = uMip;
        return tex.id;
    }

    // Helper function for passing bgfx::TextureHandle to ImGui::Image.
    inline void Image(bgfx::TextureHandle hTexture, uint8_t uFlags,
                      uint8_t uMip, const ImVec2& _size,
                      const ImVec2& _uv0 = ImVec2(0.0f, 0.0f),
                      const ImVec2& _uv1 = ImVec2(1.0f, 1.0f),
                      const ImVec4& _tintCol = ImVec4(1.0f, 1.0f, 1.0f, 1.0f),
                      const ImVec4& _borderCol = ImVec4(0.0f, 0.0f, 0.0f, 0.0f))
    {
        Image(toId(hTexture, uFlags, uMip), _size, _uv0, _uv1, _tintCol,
              _borderCol);
    }

    // Helper function for passing bgfx::TextureHandle to ImGui::Image.
    inline void Image(bgfx::TextureHandle hTexture, const ImVec2& v2Size,
                      const ImVec2& v2UV0 = ImVec2(0.0f, 0.0f),
                      const ImVec2& v2UV1 = ImVec2(1.0f, 1.0f),
                      const ImVec4& v4TintColor = ImVec4(1.0f, 1.0f, 1.0f,
                                                         1.0f),
                      const ImVec4& v4BorderColor = ImVec4(0.0f, 0.0f, 0.0f,
                                                           0.0f))
    {
        Image(hTexture, IMGUI_FLAGS_ALPHA_BLEND, 0, v2Size, v2UV0, v2UV1,
              v4TintColor, v4BorderColor);
    }

    // Helper function for passing bgfx::TextureHandle to ImGui::ImageButton.
    inline bool ImageButton(
        bgfx::TextureHandle hTexture, uint8_t uFlags, uint8_t uMip,
        const ImVec2& v2Size, const ImVec2& _uv0 = ImVec2(0.0f, 0.0f),
        const ImVec2& v2UV1 = ImVec2(1.0f, 1.0f), int iFramePadding = -1,
        const ImVec4& v4BackgroundColor = ImVec4(0.0f, 0.0f, 0.0f, 0.0f),
        const ImVec4& v4TintColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f))
    {
        return ImageButton(toId(hTexture, uFlags, uMip), v2Size, _uv0, v2UV1,
                           iFramePadding, v4BackgroundColor, v4TintColor);
    }

    // Helper function for passing bgfx::TextureHandle to ImGui::ImageButton.
    inline bool ImageButton(
        bgfx::TextureHandle hTexture, const ImVec2& v2Size,
        const ImVec2& v2UV0 = ImVec2(0.0f, 0.0f),
        const ImVec2& v2UV1 = ImVec2(1.0f, 1.0f), int iFramePadding = -1,
        const ImVec4& v4BackgroundColor = ImVec4(0.0f, 0.0f, 0.0f, 0.0f),
        const ImVec4& v4TintColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f))
    {
        return ImageButton(hTexture, IMGUI_FLAGS_ALPHA_BLEND, 0, v2Size, v2UV0,
                           v2UV1, iFramePadding, v4BackgroundColor,
                           v4TintColor);
    }

    inline void NextLine()
    {
        SetCursorPosY(GetCursorPosY() + GetTextLineHeightWithSpacing());
    }

    inline bool MouseOverArea()
    {
        return false || ImGui::IsAnyItemHovered()
               || ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);
    }

}  // namespace ImGui

#endif  // IMGUI_H_HEADER_GUARD
