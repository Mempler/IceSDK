#pragma once

#include "IceSDK/Graphics/FontManager.hh"

#include <string>

namespace IceSDK::Components
{
    struct TextComponent
    {
        // Internal use only
        uint64_t _oldTextChecksum;

        std::string m_sText;
        size_t m_sFontSize;
        Graphics::FontFaceHandle m_hFontFace;
    };
}  // namespace IceSDK::Components