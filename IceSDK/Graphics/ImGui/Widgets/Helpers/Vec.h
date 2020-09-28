#pragma once

#include <string_view>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <imgui.h>

namespace IceSDK::ImGuiWidgets::Helpers
{
    inline void DrawVec(glm::vec2 vec)
    {
        ImGui::PushItemWidth(-1);
        ImGui::InputFloat2("##label", &vec[0]);
        ImGui::PopItemWidth();
    }

    inline void DrawVec(glm::vec3 vec)
    {
        ImGui::PushItemWidth(-1);
        ImGui::InputFloat3("##label", &vec[0]);
        ImGui::PopItemWidth();
    }

    inline void DrawVec(glm::vec4 vec)
    {
        ImGui::PushItemWidth(-1);
        ImGui::InputFloat4("##label", &vec[0]);
        ImGui::PopItemWidth();
    }
}  // namespace IceSDK::ImGuiWidgets::Helpers
