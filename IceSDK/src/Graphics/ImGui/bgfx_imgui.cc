/*
 * Copyright 2014-2015 Daniel Collin. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

#include "pch.h"

#include "Graphics/ImGui/bgfx_imgui.h"

#include "Graphics/ImGui/bgfx_utils.h"

#include <bgfx/bgfx.h>
#include <bgfx/embedded_shader.h>
#include <bx/allocator.h>
#include <bx/math.h>
#include <bx/timer.h>

#include <imgui.h>

//#define USE_ENTRY 1

#ifndef USE_ENTRY
    #define USE_ENTRY 0
#endif  // USE_ENTRY

#if USE_ENTRY
    #include "../entry/entry.h"
    #include "../entry/input.h"
#endif  // USE_ENTRY

#include "Graphics/ImGui/Fonts/icons_font_awesome.ttf.h"
#include "Graphics/ImGui/Fonts/icons_kenney.ttf.h"
#include "Graphics/ImGui/Fonts/roboto_regular.ttf.h"
#include "Graphics/ImGui/Fonts/robotomono_regular.ttf.h"
#include "Graphics/ImGui/Shaders/fs_imgui_image.bin.h"
#include "Graphics/ImGui/Shaders/fs_ocornut_imgui.bin.h"
#include "Graphics/ImGui/Shaders/vs_imgui_image.bin.h"
#include "Graphics/ImGui/Shaders/vs_ocornut_imgui.bin.h"

static const bgfx::EmbeddedShader s_embeddedShaders[] = {
    BGFX_EMBEDDED_SHADER(vs_ocornut_imgui),
    BGFX_EMBEDDED_SHADER(fs_ocornut_imgui),
    BGFX_EMBEDDED_SHADER(vs_imgui_image), BGFX_EMBEDDED_SHADER(fs_imgui_image),

    BGFX_EMBEDDED_SHADER_END()
};

struct FontRangeMerge
{
    const void* data;
    size_t size;
    ImWchar ranges[3];
};

static void* MemAlloc(size_t _size, void* _userData);
static void MemFree(void* _ptr, void* _userData);

struct OcornutImguiContext
{
    void Render(ImDrawData* pDrawData) const
    {
        const auto& io = ImGui::GetIO();
        const auto width = io.DisplaySize.x;
        const auto height = io.DisplaySize.y;

        bgfx::setViewName(m_viewId, "ImGui");
        bgfx::setViewMode(m_viewId, bgfx::ViewMode::Sequential);

        auto caps = bgfx::getCaps();
        {
            float ortho[16];
            bx::mtxOrtho(ortho, 0.0f, width, height, 0.0f, 0.0f, 1000.0f, 0.0f,
                         caps->homogeneousDepth);
            bgfx::setViewTransform(m_viewId, nullptr, ortho);
            bgfx::setViewRect(m_viewId, 0, 0, uint16_t(width),
                              uint16_t(height));
        }

        // Render command lists
        for (auto ii = 0, num = pDrawData->CmdListsCount; ii < num; ++ii)
        {
            bgfx::TransientVertexBuffer tvb;
            bgfx::TransientIndexBuffer tib;

            const ImDrawList* drawList = pDrawData->CmdLists[ii];
            auto numVertices = (uint32_t) drawList->VtxBuffer.size();
            auto numIndices = (uint32_t) drawList->IdxBuffer.size();

            if (!checkAvailTransientBuffers(numVertices, m_layout, numIndices))
            {
                // not enough space in transient buffer just quit drawing the
                // rest...
                break;
            }

            bgfx::allocTransientVertexBuffer(&tvb, numVertices, m_layout);
            bgfx::allocTransientIndexBuffer(&tib, numIndices);

            auto verts = (ImDrawVert*) (void*) tvb.data;
            bx::memCopy(verts, drawList->VtxBuffer.begin(),
                        numVertices * sizeof(ImDrawVert));

            auto indices = (ImDrawIdx*) (void*) tib.data;
            bx::memCopy(indices, drawList->IdxBuffer.begin(),
                        numIndices * sizeof(ImDrawIdx));

            uint32_t offset = 0;
            for (auto cmd = drawList->CmdBuffer.begin(),
                      cmdEnd = drawList->CmdBuffer.end();
                 cmd != cmdEnd; ++cmd)
            {
                if (cmd->UserCallback) { cmd->UserCallback(drawList, cmd); }
                else if (0 != cmd->ElemCount)
                {
                    auto state = 0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A
                                 | BGFX_STATE_MSAA;

                    bgfx::TextureHandle th = m_texture;
                    bgfx::ProgramHandle program = m_program;

                    if (NULL != cmd->TextureId)
                    {
                        union
                        {
                            ImTextureID ptr;
                            struct
                            {
                                bgfx::TextureHandle handle;
                                uint8_t flags;
                                uint8_t mip;
                            } s;
                        } texture = { cmd->TextureId };
                        state |=
                            0 != (IMGUI_FLAGS_ALPHA_BLEND & texture.s.flags)
                                ? BGFX_STATE_BLEND_FUNC(
                                    BGFX_STATE_BLEND_SRC_ALPHA,
                                    BGFX_STATE_BLEND_INV_SRC_ALPHA)
                                : BGFX_STATE_NONE;
                        th = texture.s.handle;
                        if (0 != texture.s.mip)
                        {
                            const float lodEnabled[4] = { float(texture.s.mip),
                                                          1.0f, 0.0f, 0.0f };
                            bgfx::setUniform(u_imageLodEnabled, lodEnabled);
                            program = m_imageProgram;
                        }
                    }
                    else
                    {
                        state |= BGFX_STATE_BLEND_FUNC(
                            BGFX_STATE_BLEND_SRC_ALPHA,
                            BGFX_STATE_BLEND_INV_SRC_ALPHA);
                    }

                    const uint16_t xx =
                        uint16_t(bx::max(cmd->ClipRect.x, 0.0f));
                    const uint16_t yy =
                        uint16_t(bx::max(cmd->ClipRect.y, 0.0f));
                    bgfx::setScissor(
                        xx, yy,
                        uint16_t(bx::min(cmd->ClipRect.z, 65535.0f) - xx),
                        uint16_t(bx::min(cmd->ClipRect.w, 65535.0f) - yy));

                    bgfx::setState(state);
                    bgfx::setTexture(0, s_tex, th);
                    bgfx::setVertexBuffer(0, &tvb, 0, numVertices);
                    bgfx::setIndexBuffer(&tib, offset, cmd->ElemCount);
                    bgfx::submit(m_viewId, program);
                }

                offset += cmd->ElemCount;
            }
        }
    }

    void Create(const float pFontSize, bx::AllocatorI* pAllocator)
    {
        m_allocator = pAllocator;

        if (pAllocator == nullptr)
        {
            static bx::DefaultAllocator allocator;
            m_allocator = &allocator;
        }

        m_viewId = 255;
        m_lastScroll = 0;
        m_last = bx::getHPCounter();

        ImGui::SetAllocatorFunctions(MemAlloc, MemFree, nullptr);

        m_imgui = ImGui::CreateContext();

        auto& io = ImGui::GetIO();

        io.DisplaySize = ImVec2(1280.0f, 720.0f);
        io.DeltaTime = 1.0f / 60.0f;
        io.IniFilename = nullptr;

        auto type = bgfx::getRendererType();
        m_program = bgfx::createProgram(
            bgfx::createEmbeddedShader(s_embeddedShaders, type,
                                       "vs_ocornut_imgui"),
            bgfx::createEmbeddedShader(s_embeddedShaders, type,
                                       "fs_ocornut_imgui"),
            true);

        u_imageLodEnabled =
            bgfx::createUniform("u_imageLodEnabled", bgfx::UniformType::Vec4);
        m_imageProgram =
            bgfx::createProgram(bgfx::createEmbeddedShader(
                                    s_embeddedShaders, type, "vs_imgui_image"),
                                bgfx::createEmbeddedShader(
                                    s_embeddedShaders, type, "fs_imgui_image"),
                                true);

        m_layout.begin()
            .add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
            .end();

        s_tex = bgfx::createUniform("s_tex", bgfx::UniformType::Sampler);

        uint8_t* data;
        int32_t width;
        int32_t height;
        {
            ImFontConfig config;
            config.FontDataOwnedByAtlas = false;
            config.MergeMode = false;
            //			config.MergeGlyphCenterV = true;

            const ImWchar* ranges = io.Fonts->GetGlyphRangesCyrillic();
            m_font[0] = io.Fonts->AddFontFromMemoryTTF(
                (void*) s_robotoRegularTtf, sizeof(s_robotoRegularTtf),
                pFontSize, &config, ranges);
            m_font[1] = io.Fonts->AddFontFromMemoryTTF(
                (void*) s_robotoMonoRegularTtf, sizeof(s_robotoMonoRegularTtf),
                pFontSize - 3.0f, &config, ranges);

            config.MergeMode = true;
            config.DstFont = m_font[0];
        }

        io.Fonts->GetTexDataAsRGBA32(&data, &width, &height);

        m_texture =
            bgfx::createTexture2D((uint16_t) width, (uint16_t) height, false, 1,
                                  bgfx::TextureFormat::BGRA8, 0,
                                  bgfx::copy(data, width * height * 4));
    }

    void destroy()
    {
        ImGui::DestroyContext(m_imgui);

        bgfx::destroy(s_tex);
        bgfx::destroy(m_texture);

        bgfx::destroy(u_imageLodEnabled);
        bgfx::destroy(m_imageProgram);
        bgfx::destroy(m_program);

        m_allocator = NULL;
    }

    void endFrame() const
    {
        ImGui::Render();
        Render(ImGui::GetDrawData());
    }

    ImGuiContext* m_imgui;
    bx::AllocatorI* m_allocator;
    bgfx::VertexLayout m_layout;
    bgfx::ProgramHandle m_program;
    bgfx::ProgramHandle m_imageProgram;
    bgfx::TextureHandle m_texture;
    bgfx::UniformHandle s_tex;
    bgfx::UniformHandle u_imageLodEnabled;
    ImFont* m_font[3];
    int64_t m_last;
    int32_t m_lastScroll;
    bgfx::ViewId m_viewId;
};

static OcornutImguiContext s_ctx;

static void* MemAlloc(size_t _size, void* _userData)
{
    BX_UNUSED(_userData)
    return BX_ALLOC(s_ctx.m_allocator, _size);
}

static void MemFree(void* _ptr, void* _userData)
{
    BX_UNUSED(_userData)
    BX_FREE(s_ctx.m_allocator, _ptr);
}

void imguiCreate(float _fontSize, bx::AllocatorI* _allocator)
{
    s_ctx.Create(_fontSize, _allocator);
}

void imguiDestroy()
{
    s_ctx.destroy();
}

void imguiEndFrame()
{
    s_ctx.endFrame();
}

void imguiUpdateSize(int32_t _fontSize, int32_t _width, int32_t _height)
{
    ImGui::GetIO().DisplaySize = ImVec2(_width, _height);
}
