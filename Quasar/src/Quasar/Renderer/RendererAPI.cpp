#include "RendererAPI.hpp"

#include "Platform/OpenGL/OpenGLRendererAPI.hpp"

namespace Quasar
{
    
    RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

    Scope<RendererAPI> RendererAPI::create()
    {
        switch (s_API)
        {
        case RendererAPI::API::None:
            QS_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;

        case RendererAPI::API::OpenGL:
            return createScope<OpenGLRendererAPI>();

        default:
            QS_CORE_ASSERT(false, "Unknown RendererAPI!");
            return nullptr;
        }
    }

} // namespace Quasar
