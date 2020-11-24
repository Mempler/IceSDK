#include "pch.h"

#include "Graphics/ImGui/Widgets/SceneGraph.h"

#include "ECS/Components/BaseComponent.h"
#include "ECS/Components/TransformComponent.h"

#include <imgui.h>

using namespace IceSDK;
using namespace IceSDK::ImGuiWidgets;

void SceneGraph::Frame(const Memory::WeakPtr<Scene> pScene)
{
    ImGui::Begin("Scene Graph");
    {
        auto registry = pScene.lock()->GetRegistry().lock();

        for (const auto& entity :
             registry->group<Components::TransformComponent,
                             Components::BaseComponent>())
        {
            auto& base = registry->get<Components::BaseComponent>(entity);

            ImGui::Text("%s", base.name.c_str());
        }
    }
    ImGui::End();
}
