#pragma once

#include "IceSDK/Input/ButtonState.hh"
#include "IceSDK/Input/KeyboardMods.hh"
#include "IceSDK/Input/KeyboardTable.hh"
#include "IceSDK/Input/MouseTable.hh"

#if defined(ICESDK_SDL2)
    #include <SDL2/SDL.h>
#endif

#include <glm/vec2.hpp>

#include <unordered_map>

namespace IceSDK::Input
{
    class InputPipeline
    {
    public:
        void Init();

        glm::vec2 GetMouseScrollAxis() { return this->m_v2MouseScrollAxis; }
        glm::vec2 GetMousePosition() { return this->m_v2MouseMoveDelta; }

        KeyboardMods GetKeyboardMods() { return this->m_eKeyboardMods; }

        std::unordered_map<MouseTable, ButtonState> GetMouseButtonState()
        {
            return this->m_umMouseState;
        }

        std::unordered_map<KeyboardTable, ButtonState> GetKeyboardButtonState()
        {
            return this->m_umKeyboardState;
        }

#if defined(ICESDK_SDL2)
        void PumpSDL2Event(SDL_Event& SDLEvent);
#endif

    private:
        glm::vec2 m_v2MouseScrollAxis;
        glm::vec2 m_v2MouseMoveDelta;

        KeyboardMods m_eKeyboardMods;

        std::unordered_map<MouseTable, ButtonState> m_umMouseState;
        std::unordered_map<KeyboardTable, ButtonState> m_umKeyboardState;
    };
}  // namespace IceSDK::Input
