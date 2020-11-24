#include "pch.h"

#include "Assets/TextAsset.h"

using namespace IceSDK;
using namespace IceSDK::Assets;

TextAsset::TextAsset(std::string pData) : _data(std::move(pData)) { }

std::string TextAsset::ToString() const
{
    return this->_data;
}

std::vector<uint8_t> TextAsset::ToByteArray()
{
    return std::vector<uint8_t>(this->_data.begin(), this->_data.end());
}

TextAsset TextAsset::From(std::string pName, std::vector<uint8_t> pData)
{
    return TextAsset{ std::string(pData.begin(), pData.end()) };
}
