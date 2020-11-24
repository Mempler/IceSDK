#include "pch.h"

#include "Graphics/Texture2D.hh"

#include "Utils/FileSystem.hh"
#include "Utils/Logger.hh"

using namespace IceSDK;
using namespace IceSDK::Graphics;

namespace bgfx
{
    // Hack for getting BGFX Allocator
    extern bx::AllocatorI* g_allocator;
}  // namespace bgfx

static void DeleteImageContainer(void* vpPtr, void* vpUserData)
{
    BX_UNUSED(vpPtr)
    bimg::ImageContainer* const imageContainer =
        static_cast<bimg::ImageContainer*>(vpUserData);
    bimg::imageFree(imageContainer);
}

/*****************************************************
 * Load
 *
 * Loads a texture from FS into GPU Memory
 *
 * @param svPath Path to the Texture
 *
 * @return GPU Texture wrapper (Texture2D)
 *****************************************************/
Texture2D Texture2D::Load(std::string_view svPath)
{
    return Texture2D::Load(Utils::FileSystem::GetFileName(svPath),
                           Utils::FileSystem::ReadBinaryFile(svPath));
}

/*****************************************************
 * Load
 *
 * Loads a texture from Memory into GPU Memory
 *
 * @param svName Name of the texture
 *               (Used for debugging)
 * @param cvData Pixel Data
 *
 * @return GPU Texture wrapper (Texture2D)
 *****************************************************/
Texture2D Texture2D::Load(std::string_view svName,
                          const std::vector<uint8_t>& cvData)
{
    // Parse the input image
    auto* imageContainer = bimg::imageParse(
        bgfx::g_allocator, cvData.data(), static_cast<uint32_t>(cvData.size()));

    if (imageContainer == nullptr) return {};  // Return invalid texture

    const auto* const pixelData =
        bgfx::makeRef(imageContainer->m_data, imageContainer->m_size,
                      DeleteImageContainer, imageContainer);

    // Make sure we have a valid texture
    if (!bgfx::isTextureValid(
            0, false, imageContainer->m_numLayers,
            static_cast<bgfx::TextureFormat::Enum>(imageContainer->m_format),
            bgfx::TextureFormat::RGBA4))
        return {};

    Texture2D texture;
    texture.m_thHandle = bgfx::createTexture2D(
        static_cast<uint16_t>(imageContainer->m_width),
        static_cast<uint16_t>(imageContainer->m_height),
        1 < imageContainer->m_numMips, imageContainer->m_numLayers,
        static_cast<bgfx::TextureFormat::Enum>(imageContainer->m_format),
        bgfx::TextureFormat::RGBA4, pixelData);

    texture.m_iWidth = imageContainer->m_width;
    texture.m_iHeight = imageContainer->m_height;

    if (!texture.IsValid()) return texture;  // This wont really matter tbh

    bgfx::setName(texture.m_thHandle, svName.data(), svName.length());

    return texture;
}

/*****************************************************
 * LoadRaw
 *
 * Loads a texture from Memory into GPU Memory
 *
 * @param svName Name of the texture
 *               (Used for debugging)
 * @param iWidth Texture Width
 * @param iHeight Texture Height
 * @param eTextureFormat Texture Format
 * @param cvData Pixel Data
 *
 * @return GPU Texture wrapper (Texture2D)
 *****************************************************/
Texture2D Texture2D::LoadRaw(std::string_view svName, int32_t iWidth,
                             int32_t iHeight,
                             bgfx::TextureFormat::Enum eTextureFormat,
                             const std::vector<uint8_t>& cvData)
{
    const auto* const pixelData = bgfx::makeRef(cvData.data(), cvData.size());

    Texture2D texture;
    texture.m_thHandle = bgfx::createTexture2D(
        static_cast<uint16_t>(iWidth), static_cast<uint16_t>(iHeight), false, 1,
        eTextureFormat, eTextureFormat, pixelData);

    texture.m_iWidth = iWidth;
    texture.m_iHeight = iHeight;

    if (!texture.IsValid()) return texture;  // Doesn't really matter

    bgfx::setName(texture.m_thHandle, svName.data(),
                  static_cast<uint32_t>(svName.length()));

    return texture;
}

/*****************************************************
 * Create
 *
 * Creates a Texture2D on the GPU
 *
 * @param svName Name of the texture
 *               (Used for debugging)
 * @param iWidth Texture Width
 * @param iHeight Texture Height
 * @param eTextureFormat Texture Format
 *
 * @return GPU Texture wrapper (Texture2D)
 *****************************************************/
Texture2D Texture2D::Create(std::string_view svName, int32_t iWidth,
                            int32_t iHeight,
                            bgfx::TextureFormat::Enum eTextureFormat)
{
    Texture2D texture;
    texture.m_thHandle = bgfx::createTexture2D(iWidth, iHeight, false, 1,
                                               eTextureFormat, eTextureFormat);

    if (!texture.IsValid()) return texture;  // Doesn't really matter lol

    texture.m_iWidth = iWidth;
    texture.m_iHeight = iHeight;

    bgfx::setName(texture.m_thHandle, svName.data(),
                  static_cast<uint32_t>(svName.length()));

    return texture;
}

/*****************************************************
 * Modify
 *
 * Modifies a texture at Position
 *
 * @param iPosX Position X
 * @param iPosY Position Y
 * @param iWidth Texture Width
 * @param iHeight Texture Height
 * @param eTextureFormat Texture Format
 *****************************************************/
void Texture2D::Modify(int32_t iPosX, int32_t iPosY, int32_t iWidth,
                       int32_t iHeight, const std::vector<uint8_t>& cvData,
                       bgfx::TextureFormat::Enum eTextureFormat)
{
    if (cvData.empty() || iWidth == 0 || iHeight == 0) return;

    auto pixelData = bgfx::copy(cvData.data(), cvData.size());

    // Hack for figuring out our _pitch
    size_t pitch = 0;
    switch (eTextureFormat)
    {
    case bgfx::TextureFormat::R8: pitch = 1; break;
    case bgfx::TextureFormat::RG8: pitch = 2; break;
    case bgfx::TextureFormat::RGB8: pitch = 3; break;
    case bgfx::TextureFormat::RGBA8: pitch = 4; break;
    }

    bgfx::updateTexture2D(m_thHandle, 0, 0, iPosX, iPosY, iWidth, iHeight,
                          pixelData, iHeight * pitch);
}
