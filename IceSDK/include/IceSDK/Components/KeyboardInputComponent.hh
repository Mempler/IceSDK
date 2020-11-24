#pragma once

#include "IceSDK/Input/ButtonState.hh"
#include "IceSDK/Input/KeyboardMods.hh"
#include "IceSDK/Input/KeyboardTable.hh"

#include <glm/vec2.hpp>

#include <unordered_map>

namespace IceSDK::Components
{
    struct KeyboardInputComponent
    {
        Input::KeyboardMods m_eMods;
        std::unordered_map<Input::KeyboardTable, Input::ButtonState>
            m_umKeyboardState;

        bool IsPressed(Input::KeyboardTable eBtn)
        {
            return this->m_umKeyboardState.at(eBtn)
                   == Input::ButtonState::Pressed;
        }
        bool IsReleased(Input::KeyboardTable eBtn)
        {
            return this->m_umKeyboardState.at(eBtn)
                   == Input::ButtonState::Released;
        }
    };
}  // namespace IceSDK::Components
