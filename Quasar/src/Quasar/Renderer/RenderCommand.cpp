#include "RenderCommand.hpp"

#include "Platform/OpenGL/OpenGLRendererAPI.hpp"

namespace Quasar
{
    
    Scope<RendererAPI> RenderCommand::s_RendererAPI = createScope<OpenGLRendererAPI>();

} // namespace Quasar
