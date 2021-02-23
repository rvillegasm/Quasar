#pragma once

#include "Quasar/Renderer/Framebuffer.hpp"

namespace Quasar
{
    
    class OpenGLFramebuffer : public Framebuffer
    {
    private:
        uint32_t m_RendererID = 0;
        FramebufferSpecification m_Specification;

        std::vector<FramebufferTextureSpecification> m_ColorAttachmetSpecifications;
        FramebufferTextureSpecification m_DepthAttachmentSpecification;

        std::vector<uint32_t> m_ColorAttachments;
        uint32_t m_DepthAttachment = 0;

    public:
        OpenGLFramebuffer(const FramebufferSpecification &spec);
        virtual ~OpenGLFramebuffer();

        void invalidate();

        void bind() override;
        void unbind() override;

        void resize(uint32_t width, uint32_t height) override;
        int readPixel(uint32_t attachmentIndex, int x, int y) override;

        void clearAttachment(uint32_t attachmentIndex, int value) override;

        uint32_t getColorAttachmentRendererID(uint32_t index = 0) const override { QS_CORE_ASSERT(index < m_ColorAttachments.size()); return m_ColorAttachments[index]; }

        const FramebufferSpecification &getSpecification() const override { return m_Specification; }
    };

} // namespace Quasar
