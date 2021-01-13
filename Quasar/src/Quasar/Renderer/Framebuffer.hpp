#pragma once

#include "Quasar/Core/Base.hpp"

#include <cstdint>

namespace Quasar
{
    
    struct FramebufferSpecification
    {
        uint32_t width, height;
        uint32_t samples = 1;

        bool swapChainTarget = false;
    };

    class Framebuffer
    {
    public:
        virtual void bind() = 0;
        virtual void unbind() = 0;

        virtual uint32_t getColorAttachmentRendererID() const = 0;

        virtual const FramebufferSpecification &getSpecification() const = 0;

        static Ref<Framebuffer> create(const FramebufferSpecification &spec);
    };

} // namespace Quasar
