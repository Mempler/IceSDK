#include "pch.h"

#include "Graphics/Texture2D.h"

#include "Utils/FileSystem.h"
#include "Utils/Instrumentor.h"
#include "Utils/Logger.h"

using namespace IceSDK;
using namespace IceSDK::Graphics;

IceSDK::Memory::Ptr<bx::AllocatorI> GetAllocator();

static void DeleteImageContainer(void* pPtr, void* pUserData)
{
    ICESDK_PROFILE_FUNCTION();
    BX_UNUSED(pPtr)
    auto* const imageContainer = static_cast<bimg::ImageContainer*>(pUserData);
    bimg::imageFree(imageContainer);
}

Memory::Ptr<Texture2D> Texture2D::Load(const std::string& pPath)
{
    ICESDK_PROFILE_FUNCTION();
    return Texture2D::Load(std::string(FileSystem::GetFileName(pPath)),
                           FileSystem::ReadBinaryFile(pPath));
}

Memory::Ptr<Texture2D> Texture2D::Load(const std::string& pName,
                                       const std::vector<uint8_t>& pData)
{
    ICESDK_PROFILE_FUNCTION();

    const auto bxAlloc = GetAllocator();
    if (!bxAlloc) return nullptr;

    auto tex2D = std::make_shared<Texture2D>();
    auto* imageContainer = bimg::imageParse(
        bxAlloc.get(), pData.data(), static_cast<uint32_t>(pData.size()));
    if (imageContainer == nullptr) return nullptr;

    const auto* const mem =
        bgfx::makeRef(imageContainer->m_data, imageContainer->m_size,
                      DeleteImageContainer, imageContainer);

    if (!bgfx::isTextureValid(
            0, false, imageContainer->m_numLayers,
            static_cast<bgfx::TextureFormat::Enum>(imageContainer->m_format),
            bgfx::TextureFormat::RGBA4))
        return nullptr;

    tex2D->_inner = bgfx::createTexture2D(
        static_cast<uint16_t>(imageContainer->m_width),
        static_cast<uint16_t>(imageContainer->m_height),
        1 < imageContainer->m_numMips, imageContainer->m_numLayers,
        static_cast<bgfx::TextureFormat::Enum>(imageContainer->m_format),
        bgfx::TextureFormat::RGBA4, mem);

    tex2D->_width = imageContainer->m_width;
    tex2D->_height = imageContainer->m_height;

    if (!bgfx::isValid(tex2D->_inner)) return nullptr;

    bgfx::setName(tex2D->_inner, pName.data(),
                  static_cast<uint32_t>(pName.length()));

    return tex2D;
}

Memory::Ptr<Texture2D> Texture2D::LoadRaw(const std::string& pName,
                                          size_t pWidth, size_t pHeight,
                                          bgfx::TextureFormat::Enum pTexFormat,
                                          const std::vector<uint8_t>& pData)
{
    ICESDK_PROFILE_FUNCTION();

    const auto bxAlloc = GetAllocator();
    if (!bxAlloc) return nullptr;

    auto tex2D = std::make_shared<Texture2D>();

    const auto* const mem = bgfx::makeRef(pData.data(), pData.size());

    tex2D->_inner = bgfx::createTexture2D(static_cast<uint16_t>(pWidth),
                                          static_cast<uint16_t>(pHeight), false,
                                          1, pTexFormat, pTexFormat, mem);

    tex2D->_width = pWidth;
    tex2D->_height = pHeight;

    if (!bgfx::isValid(tex2D->_inner)) return nullptr;

    bgfx::setName(tex2D->_inner, pName.data(),
                  static_cast<uint32_t>(pName.length()));

    return tex2D;
}

Memory::Ptr<Texture2D> Texture2D::Create(const std::string& pName,
                                         const size_t pWidth,
                                         const size_t pHeight,
                                         bgfx::TextureFormat::Enum pTexFormat)
{
    ICESDK_PROFILE_FUNCTION();

    auto tex2D = std::make_shared<Texture2D>();

    tex2D->_inner = bgfx::createTexture2D(pWidth, pHeight, false, 1, pTexFormat,
                                          pTexFormat);

    if (!bgfx::isValid(tex2D->_inner)) return nullptr;

    tex2D->_width = pWidth;
    tex2D->_height = pHeight;

    bgfx::setName(tex2D->_inner, pName.c_str(),
                  static_cast<uint32_t>(pName.length()));

    return tex2D;
}

void Texture2D::Modify(Math::Rectf pRect, const std::vector<uint8_t>& pBuffer,
                       bgfx::TextureFormat::Enum pTexFormat)
{
    if (pBuffer.empty() || pRect.right == 0 || pRect.bottom == 0) return;

    ICESDK_PROFILE_FUNCTION();

    auto memory = bgfx::copy(pBuffer.data(), pBuffer.size());

    // Hack for figuring out our _pitch
    size_t pitch = 0;
    switch (pTexFormat)
    {
    case bgfx::TextureFormat::R8: pitch = 1; break;
    case bgfx::TextureFormat::RG8: pitch = 2; break;
    case bgfx::TextureFormat::RGB8: pitch = 3; break;
    case bgfx::TextureFormat::RGBA8: pitch = 4; break;
    }

    bgfx::updateTexture2D(this->_inner, 0, 0, pRect.X(), pRect.Y(),
                          pRect.Width(), pRect.Height(), memory,
                          pRect.Width() * pitch);
}

bgfx::TextureHandle Texture2D::GetHandle() const
{
    ICESDK_PROFILE_FUNCTION();
    return this->_inner;
}

bgfx::TextureHandle* Texture2D::GetHandlePtr()
{
    ICESDK_PROFILE_FUNCTION();
    return &this->_inner;
}
