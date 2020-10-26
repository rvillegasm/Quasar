#include "RenderCommand.hpp"

#include "Platform/OpenGL/OpenGLRendererAPI.hpp"

namespace Quasar
{
    
    RendererAPI *RenderCommand::s_RendererAPI = new OpenGLRendererAPI;

} // namespace Quasar
