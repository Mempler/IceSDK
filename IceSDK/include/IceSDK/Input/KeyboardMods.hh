#pragma once

#include <cstdint>

namespace IceSDK::Input
{
    enum KeyboardMods : uint8_t
    {
        None = 0,
        SHIFT = 1 << 0,
        CONTROL = 1 << 1,
        ALT = 1 << 2,
        SUPER = 1 << 3, /* Windows Key */
        CAPS_LOCK = 1 << 4,
        NUM_LOCK = 1 << 5,
    };
}  // namespace IceSDK::Input
