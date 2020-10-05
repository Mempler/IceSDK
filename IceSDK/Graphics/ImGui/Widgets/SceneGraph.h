#pragma once

#include "ECS/Scene.h"

#include "Utils/Memory.h"

namespace IceSDK::ImGuiWidgets::SceneGraph
{
    void Frame(Memory::WeakPtr<Scene> pScene);

    Entity GetSelected();
}  // namespace IceSDK::ImGuiWidgets::SceneGraph
