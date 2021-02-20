#include "OpenGLFramebuffer.hpp"

#include <glad/glad.h>

namespace Quasar
{
    static const uint32_t s_MaxFramebufferSize = 8192; // TODO: thi value should depende on GPU capabilities

    namespace Utils
    {
        
        static GLenum textureTarget(bool multisampled)
        {
            return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
        }

        static void createTextures(bool multisampled, uint32_t *outID, uint32_t count)
        {
            glCreateTextures(textureTarget(multisampled), count, outID);
        }

        static void bindTexture(bool multisampled, uint32_t id)
        {
            glBindTexture(textureTarget(multisampled), id);
        }

        static void attachColorTexture(uint32_t id, int samples, GLenum internalFormat, GLenum format, uint32_t width, uint32_t height, int index)
        {
            bool multisampled = samples > 1;
            if (multisampled)
            {
                glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
            }
            else
            {
                glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            }

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, textureTarget(multisampled), id, 0);
        }

        static void attachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height)
        {
            bool multisampled = samples > 1;
            if (multisampled)
            {
                glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
            }
            else
            {
                glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            }

            glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, textureTarget(multisampled), id, 0);
        }

        static bool isDepthFormat(FramebufferTextureFormat format)
        {
            switch (format)
            {
            case FramebufferTextureFormat::DEPTH24STENCIL8:
                return true;
            
            default:
                return false;
            }
        }

    } // namespace Utils
    

    OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification &spec) 
        : m_Specification(spec)
    {
        for (auto textureSpec : m_Specification.attachments.attachments)
        {
            if (!Utils::isDepthFormat(textureSpec.textureFormat))
            {
                m_ColorAttachmetSpecifications.emplace_back(textureSpec);
            }
            else
            {
                m_DepthAttachmentSpecification = textureSpec;
            }
        }
    
        invalidate();
    }
    
    OpenGLFramebuffer::~OpenGLFramebuffer() 
    {
        glDeleteFramebuffers(1, &m_RendererID);
        glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
        glDeleteTextures(1, &m_DepthAttachment);
    }
    
    void OpenGLFramebuffer::invalidate() 
    {
        if (m_RendererID)
        {
            glDeleteFramebuffers(1, &m_RendererID);
            glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
            glDeleteTextures(1, &m_DepthAttachment);

            m_ColorAttachments.clear();
            m_DepthAttachment = 0;
        }

        glCreateFramebuffers(1, &m_RendererID);
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

        bool multisampled = m_Specification.samples > 1;

        // Attachments
        if (!m_ColorAttachmetSpecifications.empty())
        {
            m_ColorAttachments.resize(m_ColorAttachmetSpecifications.size());
            Utils::createTextures(multisampled, m_ColorAttachments.data(), m_ColorAttachments.size());

            for (size_t i = 0; i < m_ColorAttachmetSpecifications.size(); i++)
            {
                Utils::bindTexture(multisampled, m_ColorAttachments[i]); // binding to rendererIDs
                switch (m_ColorAttachmetSpecifications[i].textureFormat)
                {
                case FramebufferTextureFormat::RGBA8:
                    Utils::attachColorTexture(m_ColorAttachments[i], m_Specification.samples, GL_RGBA8, GL_RGBA, m_Specification.width, m_Specification.height, i);
                    break;

                case FramebufferTextureFormat::RED_INTEGER:
                    Utils::attachColorTexture(m_ColorAttachments[i], m_Specification.samples, GL_R32I, GL_RED_INTEGER, m_Specification.width, m_Specification.height, i);
                    break;
                
                default:
                    break;
                }
            }
        }

        if (m_DepthAttachmentSpecification.textureFormat != FramebufferTextureFormat::None)
        {
            Utils::createTextures(multisampled, &m_DepthAttachment, 1);
            Utils::bindTexture(multisampled, m_DepthAttachment);

            switch (m_DepthAttachmentSpecification.textureFormat)
            {
            case FramebufferTextureFormat::DEPTH24STENCIL8:
                Utils::attachDepthTexture(m_DepthAttachment, m_Specification.samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_Specification.width, m_Specification.height);
                break;
            
            default:
                break;
            }
        }

        if (m_ColorAttachments.size() > 1)
        {
            QS_CORE_ASSERT(m_ColorAttachments.size() <= 4); // For now, support a max of 4 color attachments
            GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
            glDrawBuffers(m_ColorAttachments.size(), buffers);
        }
        else if (m_ColorAttachments.empty())
        {
            // only depth-pass
            glDrawBuffer(GL_NONE);
        }

        QS_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    
    void OpenGLFramebuffer::bind() 
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
        glViewport(0, 0, m_Specification.width, m_Specification.height);
    }
    
    void OpenGLFramebuffer::unbind() 
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    
    void OpenGLFramebuffer::resize(uint32_t width, uint32_t height) 
    {
        if (width == 0 || height == 0 || width > s_MaxFramebufferSize || height > s_MaxFramebufferSize)
        {
            QS_CORE_WARN("Attempted to rezize framebuffer to {0}, {1}", width, height);
            return;
        }

        m_Specification.width = width;
        m_Specification.height = height;
        
        invalidate();
    }
    
    int OpenGLFramebuffer::readPixel(uint32_t attachmentIndex, int x, int y) 
    {
        QS_CORE_ASSERT(attachmentIndex < m_ColorAttachments.size());
        glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
        int pixelData;
        glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
        return pixelData;
    }

} // namespace Quasar
