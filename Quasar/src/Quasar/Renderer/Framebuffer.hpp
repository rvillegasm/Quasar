#pragma once

#include "Quasar/Core/Base.hpp"

#include <cstdint>
#include <vector>

namespace Quasar
{

    enum class FramebufferTextureFormat
    {
        None = 0,
        // Color
        RGBA8,
        RED_INTEGER,
        // Depth/Stencil
        DEPTH24STENCIL8,
        // Default depth
        Depth = DEPTH24STENCIL8
    };

    struct FramebufferTextureSpecification
    {
        FramebufferTextureFormat textureFormat = FramebufferTextureFormat::None;

        FramebufferTextureSpecification() = default;
        FramebufferTextureSpecification(FramebufferTextureFormat format)
            : textureFormat(format)
        {
        }
        // TODO: filtering and wrap
    };

    struct FramebufferAttachmentSpecification
    {
        std::vector<FramebufferTextureSpecification> attachments;

        FramebufferAttachmentSpecification() = default;
        FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
            : attachments(attachments)
        {
        }
    };

    struct FramebufferSpecification
    {
        uint32_t width = 0, height = 0;
        FramebufferAttachmentSpecification attachments;
        uint32_t samples = 1;

        bool swapChainTarget = false;
    };

    class Framebuffer
    {
    public:
        virtual ~Framebuffer() = default;

        virtual void bind() = 0;
        virtual void unbind() = 0;

        virtual void resize(uint32_t width, uint32_t height) = 0;
        virtual int readPixel(uint32_t attachmentIndex, int x, int y) = 0;

        virtual uint32_t getColorAttachmentRendererID(uint32_t index = 0) const = 0;

        virtual const FramebufferSpecification &getSpecification() const = 0;

        static Ref<Framebuffer> create(const FramebufferSpecification &spec);
    };

} // namespace Quasar
