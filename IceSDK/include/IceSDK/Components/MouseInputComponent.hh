#pragma once

#include "IceSDK/Input/ButtonState.hh"
#include "IceSDK/Input/MouseTable.hh"

#include <glm/vec2.hpp>

#include <unordered_map>

namespace IceSDK::Components
{
    struct MouseInputComponent
    {
        glm::vec2 m_v2ScrollAxis;
        glm::vec2 m_v2Position;

        std::unordered_map<Input::MouseTable, Input::ButtonState>
            m_umMouseButtonState;

        bool IsPressed(Input::MouseTable eBtn)
        {
            return this->m_umMouseButtonState.at(eBtn)
                   == Input::ButtonState::Pressed;
        }
        bool IsReleased(Input::MouseTable eBtn)
        {
            return this->m_umMouseButtonState.at(eBtn)
                   == Input::ButtonState::Released;
        }
    };
}  // namespace IceSDK::Components
