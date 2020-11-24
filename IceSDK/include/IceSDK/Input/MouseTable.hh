#pragma once

#include <cstdint>

namespace IceSDK::Input
{
    enum class MouseTable : uint8_t
    {
        BTN_1 = 0,
        BTN_2 = 1,
        BTN_3 = 2,
        BTN_4 = 3,
        BTN_5 = 4,
        BTN_6 = 5,
        BTN_7 = 6,
        BTN_8 = 7,
        BTN_LAST = BTN_1,
        BTN_LEFT = BTN_2,
        BTN_RIGHT = BTN_3,
        BTN_MIDDLE = BTN_4,
    };
}  // namespace IceSDK::Input
