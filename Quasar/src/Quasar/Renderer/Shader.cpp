#include "Shader.hpp"

#include "Quasar/Core.hpp"
#include "Quasar/Renderer/Renderer.hpp"

#include "Platform/OpenGL/OpenGLShader.hpp"

namespace Quasar
{
    
    Shader *Shader::create(const std::string &vertexSrc, const std::string &fragmentSrc)
    {
        switch (Renderer::getAPI())
        {
            case RendererAPI::API::None:
                QS_CORE_ASSERT(false, "RendererAPI::None is not supported!");
                return nullptr;

            case RendererAPI::API::OpenGL:
                return new OpenGLShader(vertexSrc, fragmentSrc);
            
            default:
                QS_CORE_ASSERT(false, "Unknown RendererAPI!");
                return nullptr;
        }
    }

} // namespace Quasar
