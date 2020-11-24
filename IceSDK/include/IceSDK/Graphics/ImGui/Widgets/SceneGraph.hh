#pragma once

#include "IceSDK/Scene.hh"

namespace IceSDK::ImGuiWidgets::SceneGraph
{
    void Frame(std::weak_ptr<Scene> pScene);
}
