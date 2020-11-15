#include "GraphicsContext.hpp"

#include "Quasar/Renderer/Renderer.hpp"

#include "Platform/OpenGL/OpenGLContext.hpp"

namespace Quasar
{
    
    Scope<GraphicsContext> GraphicsContext::create(void *window) 
    {
        switch (Renderer::getAPI())
        {
        case RendererAPI::API::None:
            QS_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        
        case RendererAPI::API::OpenGL:
            return createScope<OpenGLContext>(static_cast<GLFWwindow *>(window));

        default:
            QS_CORE_ASSERT(false, "Unknown RendererAPI!");
            return nullptr;
        }
    }

} // namespace Quasar
