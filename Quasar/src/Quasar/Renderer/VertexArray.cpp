#include "VertexArray.hpp"

#include "Quasar/Renderer/Renderer.hpp"

#include "Platform/OpenGL/OpenGLVertexArray.hpp"

namespace Quasar
{
    
    Ref<VertexArray> VertexArray::create()
    {
        switch (Renderer::getAPI())
        {
        case RendererAPI::API::None:
            QS_CORE_ASSERT(false, "RendererAPI::None is not supported!");
            return nullptr;

        case RendererAPI::API::OpenGL:
            return createRef<OpenGLVertexArray>();
        
        default:
            QS_CORE_ASSERT(false, "Unknown RendererAPI!");
            return nullptr;
        }
    }

} // namespace Quasar
