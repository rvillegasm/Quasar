#include "VertexArray.hpp"

#include "Quasar/Core.hpp"
#include "Quasar/Renderer/Renderer.hpp"

#include "Platform/OpenGL/OpenGLVertexArray.hpp"

namespace Quasar
{
    
    VertexArray *VertexArray::create()
    {
        switch (Renderer::getAPI())
        {
        case RendererAPI::None:
            QS_CORE_ASSERT(false, "RendererAPI::None is not supported!");
            return nullptr;

        case RendererAPI::OpenGL:
            return new OpenGLVertexArray();
        
        default:
            QS_CORE_ASSERT(false, "Unknown RendererAPI!");
            return nullptr;
        }
    }

} // namespace Quasar
