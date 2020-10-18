/*
    DON'T USER SMART POINTERS, we literally manage the memory here
*/

#pragma once
#include "Graphics/Texture2D.h"

#include <bgfx/bgfx.h>

namespace IceSDK::Graphics
{
    struct Node
    {
        Node(uint32_t _x, uint32_t _y, uint32_t _width) :
            x(_x), y(_y), width(_width)
        {
        }

        uint32_t x;
        uint32_t y;
        uint32_t width;
    };

    class Packer
    {
    public:
        Packer();
        Packer(uint32_t pWidth, uint32_t pHeight);
        bool Push(uint32_t pWidth, uint32_t pHeight, uint32_t& pXOut,
                  uint32_t& pYOut);

        uint32_t UsedSurface();
        uint32_t TotalSurface();
        float UsageRatio();
        void Clear();

    private:
        int Fit(int index, uint32_t pWidth, uint32_t pHeight);
        void Merge();

        uint32_t _width;
        uint32_t _height;
        uint32_t _usedSpace;
        std::vector<Node> _skyline;
    };
}  // namespace IceSDK::Graphics