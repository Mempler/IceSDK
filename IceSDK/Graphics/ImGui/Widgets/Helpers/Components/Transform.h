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

            auto modelMatrixOpen = false;
            { /* Labels */
                ImGui::Text("Position");
                ImGui::Text("Scale");

                modelMatrixOpen = ImGui::TreeNodeEx("Model Matrix");
            }

            ImGui::NextColumn();

            { /* Values */
                Helpers::DrawVec(transform.position);
                Helpers::DrawVec(transform.scale);
            }

            ImGui::NextColumn();

            ImGui::Columns(1);

            if (modelMatrixOpen)
            {
                ImGui::Separator();

                Helpers::DrawVec(transform.model_matrix[0]);
                Helpers::DrawVec(transform.model_matrix[1]);
                Helpers::DrawVec(transform.model_matrix[2]);
                Helpers::DrawVec(transform.model_matrix[3]);

                ImGui::TreePop();
            }

            ImGui::TreePop();
        }
    }
}  // namespace IceSDK::ImGuiWidgets::Helpers::Components
