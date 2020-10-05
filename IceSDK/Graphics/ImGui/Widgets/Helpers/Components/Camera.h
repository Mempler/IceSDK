#pragma once

#include "ECS/Components/CameraComponent.h"

#include "Graphics/ImGui/Widgets/Helpers/Components/Transform.h"
#include "Graphics/ImGui/Widgets/Helpers/Vec.h"

#include <imgui.h>

namespace IceSDK::ImGuiWidgets::Helpers::Components
{
    inline void DrawCamera(IceSDK::Components::CameraComponent& camera)
    {
        auto open = ImGui::TreeNodeEx("Camera", ImGuiTreeNodeFlags_DefaultOpen);
        ImGui::NextColumn();

        if (open)
        {
            ImGui::NextColumn();

            { /* Labels */
                ImGui::Text("Render Mode");
                // ImGui::Text("FOV");
            }

            ImGui::NextColumn();

            { /* Values */
                std::string_view selected =
                    "Orthographic";  // There is no perspective atm

                if (camera.render_mode
                    == IceSDK::Components::eCameraRenderingMode::Perspective)
                    selected = "Perspective";

                if (ImGui::BeginCombo("##Render Mode", selected.data()))
                {
                    bool is_selected = false;

                    if (ImGui::Selectable("Perspective", is_selected))
                        camera.render_mode = IceSDK::Components::
                            eCameraRenderingMode::Perspective;

                    if (ImGui::Selectable("Orthographic", is_selected))
                        camera.render_mode = IceSDK::Components::
                            eCameraRenderingMode::Orthographic;

                    if (is_selected) ImGui::SetItemDefaultFocus();
                    ImGui::EndCombo();
                }
            }

            ImGui::NextColumn();
            ImGui::TreePop();
        }
    }
}  // namespace IceSDK::ImGuiWidgets::Helpers::Components
