#pragma once

#include "Quasar/Renderer/Framebuffer.hpp"

namespace Quasar
{
    
    class OpenGLFramebuffer : public Framebuffer
    {
    private:
        uint32_t m_RendererID = 0;
        uint32_t m_ColorAttachment = 0;
        uint32_t m_DepthAttachment = 0;
        FramebufferSpecification m_Specification;

    public:
        OpenGLFramebuffer(const FramebufferSpecification &spec);
        virtual ~OpenGLFramebuffer();

        void invalidate();

        void bind() override;
        void unbind() override;

        void resize(uint32_t width, uint32_t height) override;

        uint32_t getColorAttachmentRendererID() const override { return m_ColorAttachment; }

        const FramebufferSpecification &getSpecification() const override { return m_Specification; }
    };

} // namespace Quasar
