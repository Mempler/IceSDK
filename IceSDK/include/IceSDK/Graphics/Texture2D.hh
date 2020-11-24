#pragma once

#include <bgfx/bgfx.h>
#include <bx/allocator.h>

#include <string>
#include <string_view>
#include <vector>

namespace IceSDK::Graphics
{
    // Low level texture wrapper
    class Texture2D
    {
    private:
        int32_t m_iWidth = 0;
        int32_t m_iHeight = 0;

        bgfx::TextureHandle m_thHandle = BGFX_INVALID_HANDLE;

    public:
        ~Texture2D()
        {
            // Destroy GPU Texture Handle
            if (bgfx::isValid(this->m_thHandle))
                bgfx::destroy(this->m_thHandle);
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
        static Texture2D Load(std::string_view svPath);

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
        static Texture2D Load(std::string_view svName,
                              const std::vector<uint8_t>& cvData);

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
        static Texture2D LoadRaw(std::string_view svName, int32_t iWidth,
                                 int32_t iHeight,
                                 bgfx::TextureFormat::Enum eTextureFormat,
                                 const std::vector<uint8_t>& cvData);

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
        static Texture2D Create(std::string_view svName, int32_t iWidth,
                                int32_t iHeight,
                                bgfx::TextureFormat::Enum eTextureFormat);

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
        void Modify(int32_t iPosX, int32_t iPosY, int32_t iWidth,
                    int32_t iHeight, const std::vector<uint8_t>& cvData,
                    bgfx::TextureFormat::Enum eTextureFormat);

        /*****************************************************
         * GetWidth
         *
         * Gets the current Texture Width
         *
         * @return Texture Width
         *****************************************************/
        uint32_t GetWidth() const { return m_iWidth; }

        /*****************************************************
         * GetHeight
         *
         * Gets the current Texture Height
         *
         * @return Texture Height
         *****************************************************/
        uint32_t GetHeight() const { return m_iHeight; }

        /*****************************************************
         * GetHandle
         *
         * Gets the active texture Handle (for internal use)
         *
         * @return Texture Handle
         *****************************************************/
        bgfx::TextureHandle GetHandle() const { return m_thHandle; }

        /*****************************************************
         * IsValid
         *
         * Checks if the current texture is valid
         *
         * @return False if invalid
         *****************************************************/
        bool IsValid() { return bgfx::isValid(m_thHandle); }

        bool operator==(const Texture2D& other)
        {
            return m_thHandle.idx == ((Texture2D&) other).m_thHandle.idx;
        }
    };
}  // namespace IceSDK::Graphics
