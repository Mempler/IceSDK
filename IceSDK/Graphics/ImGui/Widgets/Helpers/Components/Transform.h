#pragma once

#include "ECS/Components/TransformComponent.h"

#include "Graphics/ImGui/Widgets/Helpers/Components/Transform.h"
#include "Graphics/ImGui/Widgets/Helpers/Vec.h"

#include <imgui.h>

namespace IceSDK::ImGuiWidgets::Helpers::Components
{
    inline void DrawTransform(
        const IceSDK::Components::TransformComponent& transform)
    {
        auto open =
            ImGui::TreeNodeEx("Transform", ImGuiTreeNodeFlags_DefaultOpen);
        ImGui::NextColumn();

        if (open)
        {
            ImGui::NextColumn();

            { /* Labels */
                ImGui::Text("Position");
                ImGui::Text("Scale");
            }

            ImGui::NextColumn();

            { /* Values */
                Helpers::DrawVec(transform.position);
                Helpers::DrawVec(transform.scale);
            }

            ImGui::NextColumn();
            ImGui::TreePop();
        }
    }
}  // namespace IceSDK::ImGuiWidgets::Helpers::Components
