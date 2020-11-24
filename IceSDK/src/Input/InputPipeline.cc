#include "pch.h"

#include "Input/InputPipeline.hh"

#include "Utils/Logger.hh"

#include "GameBase.hh"

using namespace IceSDK;
using namespace IceSDK::Input;

static InputPipeline* g_InputPipeline;

#if defined(ICESDK_SDL2)
void InputPipeline::PumpSDL2Event(SDL_Event& SDLEvent)
{
    if (SDLEvent.type == SDL_KEYDOWN || SDLEvent.type == SDL_KEYUP)
    {
        auto keyEvent = SDLEvent.key.keysym;

        uint8_t keyboardMods = KeyboardMods::None;

        if ((keyEvent.mod & KMOD_LSHIFT) > 0)
        {
            keyboardMods |= KeyboardMods::SHIFT;
        }

        if ((keyEvent.mod & KMOD_RSHIFT) > 0)
        {
            keyboardMods |= KeyboardMods::SHIFT;
        }

        if ((keyEvent.mod & KMOD_LCTRL) > 0)
        {
            keyboardMods |= KeyboardMods::CONTROL;
        }

        if ((keyEvent.mod & KMOD_RCTRL) > 0)
        {
            keyboardMods |= KeyboardMods::CONTROL;
        }

        if ((keyEvent.mod & KMOD_RALT) > 0)
        {
            keyboardMods |= KeyboardMods::ALT;
        }

        if ((keyEvent.mod & KMOD_LALT) > 0)
        {
            keyboardMods |= KeyboardMods::ALT;
        }

        if ((keyEvent.mod & KMOD_LGUI) > 0)
        {
            keyboardMods |= KeyboardMods::SUPER;
        }

        if ((keyEvent.mod & KMOD_RGUI) > 0)
        {
            keyboardMods |= KeyboardMods::SUPER;
        }

        if ((keyEvent.mod & KMOD_NUM) > 0)
        {
            keyboardMods |= KeyboardMods::NUM_LOCK;
        }

        if ((keyEvent.mod & KMOD_CAPS) > 0)
        {
            keyboardMods |= KeyboardMods::CAPS_LOCK;
        }

        this->m_eKeyboardMods = (KeyboardMods) keyboardMods;
    }

    if (SDLEvent.type == SDL_KEYDOWN)
    {
        auto keyEvent = SDLEvent.key.keysym;

        this->m_umKeyboardState.insert_or_assign(
            (KeyboardTable) SDL_SCANCODE_TO_KEYCODE(keyEvent.scancode),
            ButtonState::Pressed);
    }

    if (SDLEvent.type == SDL_KEYUP)
    {
        auto keyEvent = SDLEvent.key.keysym;

        this->m_umKeyboardState.insert_or_assign(
            (KeyboardTable) SDL_SCANCODE_TO_KEYCODE(keyEvent.scancode),
            ButtonState::Released);
    }

    if (SDLEvent.type == SDL_MOUSEMOTION)
    {
        auto motionEvent = SDLEvent.motion;

        this->m_v2MouseMoveDelta.x = motionEvent.x;
        this->m_v2MouseMoveDelta.y = motionEvent.y;
    }

    if (SDLEvent.type == SDL_MOUSEWHEEL)
    {
        auto scrollEvent = SDLEvent.wheel;

        this->m_v2MouseScrollAxis.x = scrollEvent.x;
        this->m_v2MouseScrollAxis.y = scrollEvent.y;
    }

    if (SDLEvent.type == SDL_MOUSEBUTTONDOWN)
    {
        auto mouseButtonEvent = SDLEvent.button;

        this->m_umMouseState.insert_or_assign(
            (MouseTable)(mouseButtonEvent.button + 1), ButtonState::Pressed);
    }

    if (SDLEvent.type == SDL_MOUSEBUTTONUP)
    {
        auto mouseButtonEvent = SDLEvent.button;

        this->m_umMouseState.insert_or_assign(
            (MouseTable)(mouseButtonEvent.button + 1), ButtonState::Released);
    }
}
#endif

void InputPipeline::Init() { }
