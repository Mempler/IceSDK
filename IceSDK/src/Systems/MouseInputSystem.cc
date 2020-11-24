#include "pch.hh"

#include "Input/Components/MouseInputComponent.h"
#include "Input/MouseTable.h"
#include "Utils/Instrumentor.h"

#include "GameBase.h"

using namespace IceSDK;
using namespace IceSDK::Input;
using namespace IceSDK::Systems;

void MouseInputSystem::Tick(float pDelta)
{
    const auto registry = this->_registry.lock();
    if (registry == nullptr) return;

    auto inputPipeline = GetGameBase()->GetInputPipeline();

    // Calculate Axis
    glm::vec2 Axis = inputPipeline->GetMouseScrollAxis();

    // Calculate Delta
    glm::vec2 Position = inputPipeline->GetMousePosition();

    auto mouseInputGroup =
        registry->view<Input::Components::MouseInputComponent>();
    for (auto rawMouseInputEntity : mouseInputGroup)
    {
        auto mouseEntity = Entity(this->_registry, rawMouseInputEntity);

        auto& mouse =
            mouseEntity.GetComponent<Input::Components::MouseInputComponent>();

        mouse.Position = Position;
        mouse.ScrollAxis = Axis;
        mouse.MouseButtonState = inputPipeline->GetMouseButtonState();
    }
}

void MouseInputSystem::Draw(float pDelta) { }
