#pragma once

#include "Quasar/Core/Core.hpp"
#include "Quasar/Renderer/Texture.hpp"

#include <glad/glad.h>

namespace Quasar
{
    
    class OpenGLTexture2D final : public Texture2D
    {
    private:
        std::string m_Path;
        uint32_t m_Width;
        uint32_t m_Height;
        uint32_t m_RendererID;
        GLenum m_InternalFormat;
        GLenum m_DataFormat;

    public:
        OpenGLTexture2D(const std::string &path);
        OpenGLTexture2D(uint32_t width, uint32_t height);
        ~OpenGLTexture2D();

        uint32_t getWidth() const override { return m_Width; }
        uint32_t getheight() const override { return m_Height; }

        void setData(void *data, uint32_t size) override;

        void bind(uint32_t slot = 0) const override;

        bool operator==(const Texture &other) const override
        { 
            return m_RendererID == ((OpenGLTexture2D&)other).m_RendererID;
        };
    };

} // namespace Quasar
