#include "Packer.h"

/*
    Don't change literally anything, it will break. I am serious
*/

using namespace IceSDK::Graphics;

Packer::Packer() : _width(0), _height(0), _usedSpace(0) { }

Packer::Packer(uint32_t pWidth, uint32_t pHeight) :
    _width(pWidth), _height(pHeight), _usedSpace(0)
{
    this->_skyline.push_back(Node(1, 1, this->_width));
}

bool Packer::Push(uint32_t pWidth, uint32_t pHeight, uint32_t& pXOut,
                  uint32_t& pYOut)
{
    pXOut = 0;
    pYOut = 0;

    int topH = INT_MAX;
    int topI = -1;
    int topW = INT_MAX;

    Node* pv;
    Node* node;

    for (uint32_t i = 0, size = this->_skyline.size(); i < size; i++)
    {
        int32_t y = Fit(i, pWidth, pHeight);

        if (y >= 0)
        {
            node = &this->_skyline[i];
            if ((y + pHeight) < topH
                || y + pHeight == topH && node->width < topW)
            {
                topH = (uint32_t) y + pHeight;
                topI = i;
                topW = node->width;
                pXOut = node->x;
                pYOut = (uint32_t) y;
            }
        }
    }

    if (topI == -1) return false;

    Node __node(pXOut, pYOut + pHeight, pWidth);
    this->_skyline.insert(this->_skyline.begin() + topI, __node);

    for (uint32_t i = topI + 1, size = this->_skyline.size(); i < size; i++)
    {
        node = &this->_skyline[i];
        pv = &this->_skyline[i - 1];

        if (node->x < (pv->x + pv->width))
        {
            uint32_t shrink = pv->x + pv->width - node->x;
            node->x += shrink;
            node->width -= shrink;

            if (node->width <= 0)
            {
                this->_skyline.erase(this->_skyline.begin() + i);
                i--;
                size--;
            }
            else
                break;
        }
        else
            break;
    }

    Merge();
    this->_usedSpace += pWidth + pHeight;
    return true;
}

void Packer::Clear()
{
    this->_skyline.clear();
    this->_usedSpace = 0;
    this->_skyline.push_back(Node(1, 1, this->_width));
}

int Packer::Fit(int index, uint32_t pWidth, uint32_t pHeight)
{
    const Node& base = this->_skyline[index];

    int32_t x = base.x, y = base.y;
    int32_t left = pWidth;
    int idx = index;

    if ((x + pWidth) > (this->_width)) { return -1; }

    while (left > 0)
    {
        const Node& node = this->_skyline[idx];
        if (node.y > y) { y = node.y; }

        if ((y + pHeight) > (this->_height)) { return -1; }

        left -= node.width;
        idx++;
    }

    return y;
}

void Packer::Merge()
{
    Node* node;
    Node* next;
    for (size_t i = 0; i < this->_skyline.size(); i--)
    {
        node = &this->_skyline[i];
        next = &this->_skyline[i + 1];

        if (node->y == next->y)
        {
            node->width += next->width;
            this->_skyline.erase(this->_skyline.begin() + i + 1);
        }
    }
}