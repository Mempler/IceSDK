#pragma once

#include <bgfx/bgfx.h>

#include <cstdio>

/*
 * Copyright 2011-2020 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

/// Returns true if both internal transient index and vertex buffer have
/// enough space.
///
/// @param[in] _numVertices Number of vertices.
/// @param[in] _layout Vertex layout.
/// @param[in] _numIndices Number of indices.
///
inline bool checkAvailTransientBuffers(uint32_t uNumVertices,
                                       const bgfx::VertexLayout& vLayout,
                                       uint32_t uNumIndices)
{
    return uNumVertices
               == bgfx::getAvailTransientVertexBuffer(uNumVertices, vLayout)
           && (0 == uNumIndices
               || uNumIndices
                      == bgfx::getAvailTransientIndexBuffer(uNumIndices));
}
