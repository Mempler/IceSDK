#pragma once

#include <bgfx/bgfx.h>

namespace IceSDK::Components
{
    struct MeshComponent
    {
        bgfx::VertexBufferHandle m_hVertexBuffer;
        bgfx::IndexBufferHandle m_hIndexBuffer;
    };
}  // namespace IceSDK::Components
