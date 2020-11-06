#pragma once

#include "Quasar/Core/Core.hpp"
#include "Quasar/Renderer/Texture.hpp"

namespace Quasar
{
    
    class OpenGLTexture2D final : public Texture2D
    {
    private:
        std::string m_Path;
        uint32_t m_Width;
        uint32_t m_Height;
        uint32_t m_RendererID;

    public:
        OpenGLTexture2D(const std::string &path);
        ~OpenGLTexture2D();

        uint32_t getWidth() const override { return m_Width; }
        uint32_t getheight() const override { return m_Height; }

        void bind(uint32_t slot = 0) const override;  
    };

} // namespace Quasar
