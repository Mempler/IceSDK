#pragma once

#include <string>

namespace IceSDK::ECS::Components
{
    struct BaseComponent
    {
        std::string m_sName;
        bool m_bIsEnabled;
    };
}  // namespace IceSDK::ECS::Components
