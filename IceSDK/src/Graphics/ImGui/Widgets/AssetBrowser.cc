#include "pch.h"

#include "Graphics/ImGui/Widgets/AssetBrowser.h"

#include "Graphics/ImGui/Widgets/AssetBrowserResources/animated_texture.png.h"
#include "Graphics/ImGui/Widgets/AssetBrowserResources/audio.png.h"
#include "Graphics/ImGui/Widgets/AssetBrowserResources/particle.png.h"
#include "Graphics/ImGui/Widgets/AssetBrowserResources/texture.png.h"
#include "Graphics/ImGui/Widgets/AssetBrowserResources/unknown.png.h"
#include "Graphics/ImGui/Widgets/AssetBrowserResources/video.png.h"
#include "Graphics/Texture2D.h"

#include <imgui.h>

using namespace IceSDK;
using namespace IceSDK::ImGuiWidgets;

std::vector<Memory::Ptr<Graphics::Texture2D>> g_CachedTexture;

void ShowToolTip(Assets::AssetFile*, Assets::AssetRegion* pEntry)
{
    if (!ImGui::IsItemHovered()) return;

    ImGui::BeginTooltip();
    {
        auto textWidth = ImGui::CalcTextSize(pEntry->name).x;
        ImGui::Text("Path:");
        ImGui::SameLine(400.0f - textWidth);
        ImGui::Text(pEntry->name);

        const auto sizeInText = Memory::SizeInText(pEntry->size);
        textWidth = ImGui::CalcTextSize(sizeInText.c_str()).x;
        ImGui::Text("Size In Memory:");
        ImGui::SameLine(400.0f - textWidth);
        ImGui::Text(sizeInText.c_str());

        const auto* typeString = "";
        switch (pEntry->type)
        {
        case Assets::eAssetType::Texture: typeString = "Texture"; break;
        case Assets::eAssetType::AnimatedTexture:
            typeString = "AnimatedTexture";
            break;
        case Assets::eAssetType::Audio: typeString = "Audio"; break;
        case Assets::eAssetType::Video: typeString = "Video"; break;
        case Assets::eAssetType::Particle: typeString = "Particle"; break;
        case Assets::eAssetType::Shader: typeString = "Particle"; break;

        case Assets::eAssetType::Text: typeString = "Text"; break;
        default: typeString = "Unknown"; break;
        }

        textWidth = ImGui::CalcTextSize(typeString).x;
        ImGui::Text("Type:");
        ImGui::SameLine(400.0f - textWidth);
        ImGui::Text(typeString);
    }
    ImGui::EndTooltip();
}

void AssetBrowser::Init(Memory::Ptr<Assets::AssetManager> pManager)
{
    g_CachedTexture.push_back(Graphics::Texture2D::Load(
        "Unknown",
        std::vector<uint8_t>(&unknown_png[0], &unknown_png[unknown_png_len])));
    g_CachedTexture.push_back(Graphics::Texture2D::Load(
        "Texture",
        std::vector<uint8_t>(&texture_png[0], &texture_png[texture_png_len])));
    g_CachedTexture.push_back(Graphics::Texture2D::Load(
        "AnimatedTexture",
        std::vector<uint8_t>(&animated_texture_png[0],
                             &animated_texture_png[animated_texture_png_len])));
    g_CachedTexture.push_back(Graphics::Texture2D::Load(
        "Audio",
        std::vector<uint8_t>(&audio_png[0], &audio_png[audio_png_len])));
    g_CachedTexture.push_back(Graphics::Texture2D::Load(
        "Video",
        std::vector<uint8_t>(&video_png[0], &video_png[video_png_len])));
    g_CachedTexture.push_back(Graphics::Texture2D::Load(
        "Particle", std::vector<uint8_t>(&particle_png[0],
                                         &particle_png[particle_png_len])));
    // TODO:
    g_CachedTexture.push_back(Graphics::Texture2D::Load(
        "Shader",
        std::vector<uint8_t>(&unknown_png[0], &unknown_png[unknown_png_len])));
    g_CachedTexture.push_back(Graphics::Texture2D::Load(
        "Text",
        std::vector<uint8_t>(&unknown_png[0], &unknown_png[unknown_png_len])));
}

void AssetBrowser::Frame(Memory::Ptr<Assets::AssetManager> pManager)
{
    ImGui::SetNextWindowSize(ImVec2(750, 300));
    ImGui::Begin("Asset Browser");
    {
        auto window_visible_x2 =
            ImGui::GetWindowPos()[0] + ImGui::GetContentRegionMax()[0];

        // Left view of the asset browser
        // the "File tree"
        ImGui::BeginChild("AssetBrowser::FileTree", ImVec2(200, -1));
        {
            auto i = 0;
            auto off = 0.0f;
            for (auto& assetFile : pManager->GetAll())
            {
                ImGui::Text("%i:", i);

                off += 10.0f;

                for (auto& assetEntry : assetFile->header.asset_register)
                {
                    const auto cursorPosX = ImGui::GetCursorPosX();
                    ImGui::SetCursorPosX(cursorPosX + off);

                    ImGui::Text(assetEntry.name);

                    ShowToolTip(assetFile.get(), &assetEntry);
                }

                i += 1;
            }
        }
        ImGui::EndChild();

        ImGui::SameLine();

        ImGui::BeginChild("AssetBrowser::FileView", ImVec2(-1, -1));
        {
            int i = 0;
            for (auto& assetFile : pManager->GetAll())
            {
                for (auto& assetEntry : assetFile->header.asset_register)
                {
                    if (std::string(assetEntry.name).empty()) break;

                    auto typeTexture =
                        g_CachedTexture.at((size_t) assetEntry.type);

                    auto last_button_x2 = ImGui::GetItemRectMax()[0];
                    auto next_button_x2 = last_button_x2
                                          + ImGui::GetStyle().ItemSpacing[0]
                                          + 96.0f;

                    if (i < Assets::asset_register_size
                        && next_button_x2 < window_visible_x2)
                    { ImGui::SameLine(); }

                    auto keySize = ImGui::CalcTextSize(assetEntry.name, nullptr,
                                                       false, 0.0f);
                    ImGui::BeginChild(
                        (std::string(assetEntry.name) + "/content").c_str(),
                        ImVec2(96.0f, 106.0f + keySize.y), false,
                        ImGuiWindowFlags_AlwaysAutoResize);
                    {
                        ImGui::Image((ImTextureID) typeTexture->GetHandle().idx,
                                     ImVec2(96, 96));

                        ImGui::Text(assetEntry.name);  // TODO: trim
                    }
                    ImGui::EndChild();

                    i++;
                }
            }
        }
        ImGui::EndChild();
    }
    ImGui::End();
}
