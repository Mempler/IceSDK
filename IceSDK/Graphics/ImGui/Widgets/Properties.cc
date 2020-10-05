#include "Graphics/ImGui/Widgets/Properties.h"

#include "GameBase.h"
#include "Graphics/ImGui/Widgets/Helpers/Components/Camera.h"
#include "Graphics/ImGui/Widgets/Helpers/Components/Transform.h"
#include "Graphics/ImGui/Widgets/Helpers/Vec.h"

using namespace IceSDK;
using namespace IceSDK::ImGuiWidgets;

void Properties::Init() { }

void Properties::Frame()
{
    auto scene = GetGameBase()->GetActiveScene().lock();
    if (scene == nullptr) return;

    ImGui::SetNextWindowSize(ImVec2(430, 450), ImGuiCond_FirstUseEver);
    ImGui::Begin("Properties");

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
    ImGui::Columns(2);
    ImGui::Separator();

    {
        Helpers::Components::DrawTransform(Components::TransformComponent{
            { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, glm::mat4(0) });

        auto cam = Components::CameraComponent{
            Components::eCameraRenderingMode::Orthographic,
            true,
            { 0.0, 0.0 },
            glm::mat4(0),
            glm::mat4(0)
        };

        Helpers::Components::DrawCamera(cam);
    }

    ImGui::Columns(1);
    ImGui::Separator();
    ImGui::PopStyleVar();
    ImGui::End();
}
