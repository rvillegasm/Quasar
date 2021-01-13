#include "Framebuffer.hpp"

#include "Quasar/Renderer/Renderer.hpp"

#include "Platform/OpenGL/OpenGLFramebuffer.hpp"

namespace Quasar
{
    Ref<Framebuffer> Framebuffer::create(const FramebufferSpecification &spec) 
    {
        switch (Renderer::getAPI())
        {
        case RendererAPI::API::None:
            QS_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;

        case RendererAPI::API::OpenGL:
            return createRef<OpenGLFramebuffer>(spec);

        default:
            QS_CORE_ASSERT(false, "Unknown RendererAPI!");
            return nullptr;
        }
    }

    


} // namespace Quasar
