#pragma once

#include <cstdint>

namespace IceSDK::Input
{
    enum class ButtonState : uint8_t
    {
        Pressed,
        Released
    };
}  // namespace IceSDK::Input
