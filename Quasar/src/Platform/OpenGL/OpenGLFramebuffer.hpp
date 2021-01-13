#pragma once

#include "Quasar/Renderer/Framebuffer.hpp"

namespace Quasar
{
    
    class OpenGLFramebuffer : public Framebuffer
    {
    private:
        uint32_t m_RendererID;
        uint32_t m_ColorAttachment;
        uint32_t m_DepthAttachment;
        FramebufferSpecification m_Specification;

    public:
        OpenGLFramebuffer(const FramebufferSpecification &spec);
        virtual ~OpenGLFramebuffer();

        void invalidate();

        void bind() override;
        void unbind() override;

        uint32_t getColorAttachmentRendererID() const override { return m_ColorAttachment; }

        const FramebufferSpecification &getSpecification() const override { return m_Specification; }
    };

} // namespace Quasar
