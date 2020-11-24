#include "pch.h"

#include "Input/Systems/KeyboardInputSystem.h"

#include "Input/Components/KeyboardInputComponent.h"
#include "Input/KeyboardMods.h"
#include "Input/KeyboardTable.h"

#include "GameBase.h"

using namespace IceSDK;
using namespace IceSDK::Input;
using namespace IceSDK::Systems;

void KeyboardInputSystem::Tick(float pDelta)
{
    const auto registry = this->_registry.lock();
    if (registry == nullptr) return;

    auto inputPipeline = GetGameBase()->GetInputPipeline();

    auto keyboardInputGroup =
        registry->view<Input::Components::KeyboardInputComponent>();
    for (auto rawKeyboardInputEntity : keyboardInputGroup)
    {
        auto keyboardEntity = Entity(this->_registry, rawKeyboardInputEntity);

        auto& keyboard =
            keyboardEntity
                .GetComponent<Input::Components::KeyboardInputComponent>();

        keyboard.Mods = inputPipeline->GetKeyboardMods();
        keyboard.KeyboardState = inputPipeline->GetKeyboardButtonState();
    }
}

void KeyboardInputSystem::Draw(float pDelta) { }
